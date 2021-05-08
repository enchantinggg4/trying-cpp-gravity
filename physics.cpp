#include "Gravity.h"

void gravity(vector<Point> &points){
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
}