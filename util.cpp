//
// Created by Yuri Stiopin on 08.05.2021.
//
#include "Gravity.h"
#include <cstdlib>
#include <ctime>

float rfloat(float max){
    return rand() / (RAND_MAX / max);
}

float rfloat(float min, float max){
    return min + rand() / (RAND_MAX / (max - min));
}

float max(float a, float maxv){
    return a > maxv ? maxv : a;
}