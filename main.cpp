


#include "Gravity.h"
#include <iostream>
#include <chrono>
#include <thread>

const float width = 1800;
const float height = 1000;

float radius(float mass) {
    float r = (mass / MAX_MASS) * MAX_RADIUS;

    return r > MAX_RADIUS ? MAX_RADIUS : r;
}

float dist(struct Vector pnt) {
    float distSq = pnt.x * pnt.x + pnt.y * pnt.y;
    float dist = sqrt(distSq);
    return dist;
}

//f = G m1*m2 / r^2
void update(vector<Point> &points, long timedelta) {
    // calculate accelerations
    for (int i = 0; i < points.size(); i++) {
        struct Point &thisPnt = points[i];
        for (int j = i + 1; j < points.size(); j++) {
            // ignore self;
            struct Point &otherPnt = points[j];



            if (otherPnt.mass == 0) continue;


            float d = dist(Vector{thisPnt.x - otherPnt.x, thisPnt.y - otherPnt.y});

            float collapse = radius(otherPnt.mass + thisPnt.mass) / 2;
            if (d < collapse) {
                // collision
                if (thisPnt.mass > otherPnt.mass) {
                    // merge
                    thisPnt.mass += otherPnt.mass;
                    thisPnt.acceleration.x += otherPnt.acceleration.x * otherPnt.mass / thisPnt.mass;
                    thisPnt.acceleration.y += otherPnt.acceleration.y * otherPnt.mass / thisPnt.mass;
                    otherPnt.mass = 0;
                } else {
                    // merge
                    otherPnt.mass += thisPnt.mass;
                    otherPnt.acceleration.x += thisPnt.acceleration.x * thisPnt.mass / otherPnt.mass;
                    otherPnt.acceleration.y += thisPnt.acceleration.y * thisPnt.mass / otherPnt.mass;
                    thisPnt.mass = 0;
                    continue;
                }
            }

            // if bad switch
            float xDiff = otherPnt.x - thisPnt.x;
            float yDiff = otherPnt.y - thisPnt.y;


            float distSq = xDiff * xDiff + yDiff * yDiff;
            distSq = distSq < DIST_TO_COLLAPSE ? DIST_TO_COLLAPSE : distSq;
            float dist = sqrt(distSq);


            float force = G * (thisPnt.mass * otherPnt.mass) / distSq;

            float forceX = (xDiff / dist) * force;
            float forceY = (yDiff / dist) * force;

            thisPnt.acceleration.x += forceX / thisPnt.mass;
            thisPnt.acceleration.y += forceY / thisPnt.mass;
        }
    }
    for (int i = 0; i < points.size(); i++) {
        struct Point &thisPnt = points[i];
        if (thisPnt.mass == 0) {
            // sadkek hope no break
            points.erase(points.begin() + i);
        }
    }

    for (auto &thisPnt : points) {
        float accLen = dist(thisPnt.acceleration);
        thisPnt.x += thisPnt.acceleration.x * accLen / timedelta;
        thisPnt.y += thisPnt.acceleration.y * accLen / timedelta;

        if (thisPnt.x > width) {
            thisPnt.x = width - fmod(thisPnt.x, width);
            thisPnt.acceleration.x *= -0.9;

        }
        if (thisPnt.x < 0) {
            thisPnt.x = fmod(-thisPnt.x, width);
            thisPnt.acceleration.x *= -0.9;
        }

        if (thisPnt.y > height) {
            thisPnt.y = height - fmod(thisPnt.y, height);
            thisPnt.acceleration.y *= -0.9;
        }
        if (thisPnt.y < 0) {
            thisPnt.y = fmod(-thisPnt.y, height);
            thisPnt.acceleration.y *= -0.9;
        }
    }


}


void setColor(sf::Shape *shape, Point p) {
    float red = max(dist(p.acceleration) / 15.0, 1);
    float green = max(p.mass / MAX_MASS, 1);
    float blue = (red + green / 2);

    float maxColorSat = 150;
    sf::Color c = sf::Color(red * maxColorSat, green * maxColorSat, blue * maxColorSat);
    shape->setFillColor(c);
}

void draw(sf::RenderWindow *window, vector<Point> &points) {
    for (auto point : points) {
        float r = radius(point.mass);
        sf::CircleShape shape(r);
        shape.setPosition(point.x, point.y);
        setColor(&shape, point);
        window->draw(shape);
    }
}

void randomPoints(vector<Point> &points, int n) {

    for (int i = 0; i < n; i++) {
        struct Vector baseAcceleration = {
                rfloat(5),
                rfloat(5)
        };
        struct Point point = {
                rfloat(width),
                rfloat(height),
                MIN_MASS + rfloat(MIN_MASS, MAX_INITIAL_MASS),
                baseAcceleration

        };
        points.push_back(point);
    }
}


[[noreturn]] void update_thread(vector<Point> &points) {
    sf::Clock clock;
    for (;;) {
        sf::Time elapsed1 = clock.getElapsedTime();
        update(points, 5.0);
        clock.restart();
        std::this_thread::sleep_for(100ms);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!", sf::Style::Resize);

    std::vector<Point> points;


    window.setSize(sf::Vector2u(1920, 1080));
    window.setFramerateLimit(60);
    randomPoints(points, POINT_COUNT);
    sf::Clock deltaClock;


    thread t1(update_thread, std::ref(points));
    t1.detach();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        draw(&window, points);
        window.display();
    }
    return 0;
}
