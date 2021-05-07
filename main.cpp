#include <SFML/Graphics.hpp>
#include "iostream"
#include <chrono>
#include <thread>

using std::vector;

const int POINT_COUNT = 2000;
const float MAX_MASS = 100;
const float MAX_INITIAL_MASS = 2;
const float MIN_MASS = 1;

const float DIST_TO_COLLAPSE = 0.5;

const float MAX_RADIUS = 20;

const float G = 1;

const float width = 1800;
const float height = 1000;
struct Vector {
    float x, y;
};

struct Point {
    float x, y, mass;
    struct Vector acceleration;

};


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
void update(vector<Point> &points) {
    // calculate accelerations
    for (int i = 0; i < points.size(); i++) {
        struct Point &thisPnt = points[i];
        for (int j = 0; j < points.size(); j++) {
            // ignore self
            if (i == j) continue;

            struct Point &otherPnt = points[j];


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

    for (auto &thisPnt : points) {
        float accLen = dist(thisPnt.acceleration);

        float timePassed = 1000.0 / 60;

        thisPnt.x += thisPnt.acceleration.x * accLen / timePassed;
        thisPnt.y += thisPnt.acceleration.y * accLen / timePassed;

        if (thisPnt.x > width) {
            thisPnt.x = fmod(thisPnt.x, width);
        }
        if (thisPnt.x < 0) {
            thisPnt.x = width - fmod(-thisPnt.x, width);
        }

        if (thisPnt.y > height) {
            thisPnt.y = fmod(thisPnt.y, height);
        }
        if (thisPnt.y < 0) {
            thisPnt.y = height - fmod(-thisPnt.y, height);
        }
    }

    for (int i = 0; i < points.size(); i++) {
        // check collision
        struct Point &thisPnt = points[i];

        if (thisPnt.mass == 0) continue;
        for (int j = 0; j < points.size(); j++) {
            struct Point &otherPnt = points[j];

            if (i == j || otherPnt.mass == 0) continue;


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
                }
            }
        }
    }

    for (int i = 0; i < points.size(); i++) {
        struct Point &thisPnt = points[i];
        if (thisPnt.mass == 0) {
            // sadkek hope no break
            points.erase(points.begin() + i);
        }
    }
}


void setColor(sf::Shape *shape, float mass) {

    if (mass > MAX_MASS / 2) {
        shape->setFillColor(sf::Color::Red);
    } else {
        shape->setFillColor(sf::Color::Green);
    }

}

void draw(sf::RenderWindow *window, vector<Point> &points) {
    for (auto point : points) {
        float r = radius(point.mass);
        sf::CircleShape shape(r);
        shape.setPosition(point.x, point.y);
        setColor(&shape, point.mass);
        window->draw(shape);
    }
}

void randomPoints(vector<Point> &points, int n) {

    for (int i = 0; i < n; i++) {
        struct Vector vec = {
                static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 50)),
                static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 50))
        };
        struct Point point = {
                static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / width)),
                static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / height)),
                MIN_MASS +
                static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (MAX_INITIAL_MASS - MIN_MASS))),
                vec

        };
        points.push_back(point);
//        points[i] = point;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");

    std::vector<Point> points;

    window.setSize(sf::Vector2u(1920, 1080));
    window.setFramerateLimit(60);
    randomPoints(points, POINT_COUNT);
    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        update(points);
        draw(&window, points);
        window.display();
    }
    return 0;
}
