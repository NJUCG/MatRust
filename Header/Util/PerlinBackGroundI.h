#pragma once
#include<glm/glm.hpp>
using namespace glm;

class PerlinBackGroundI
{
public:
    virtual float twist(vec2 pos) {
        return 0;
    }
    virtual float fade(float t) {
        return t * t * t * (t * (6 * t - 15) + 10);
    }
};

