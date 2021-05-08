//
// Created by Yuri Stiopin on 08.05.2021.
//

#ifndef GRAVITY_UTIL_H
#define GRAVITY_UTIL_H

#endif //GRAVITY_UTIL_H

#include <SFML/Graphics.hpp>
#include <thread>
using std::vector;
using namespace std;



const int POINT_COUNT = 2000;
const float MAX_MASS = 100;
const float MAX_INITIAL_MASS = 2;
const float MIN_MASS = 1;

const float DIST_TO_COLLAPSE = 0.5;

const float MAX_RADIUS = 20;

const float G = 100;

struct Vector {
    float x, y;
};

struct Point {
    float x, y, mass;
    struct Vector acceleration;

};


float max(float a, float maxv);
float rfloat(float max);
float rfloat(float min, float max);