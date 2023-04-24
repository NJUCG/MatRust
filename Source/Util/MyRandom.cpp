#include "MyRandom.h"

float MyRandom::_0_1F()
{
    return QRandomGenerator::global()->generateDouble();
}

double MyRandom::_0_1()
{
    return QRandomGenerator::global()->generateDouble();
}

vec3 MyRandom::sphere_0_1()
{
    double theta = 2 * M_PI * MyRandom::_0_1();
    double phi = 2 * M_PI * MyRandom::_0_1();
    vec3 ret = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    return ret;
}
