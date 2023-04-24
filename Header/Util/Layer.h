#pragma once
#include"ImageHelper.h"
#include<glm/glm.hpp>
#include<vector>
#include"PipelineConfig.h"
#include"FilmHeader.h"
#include"ColorHelper.h"
using namespace std;
using namespace glm;


class RustUnit
{
public:
	vec4 color;
    vec3 normal_disturb;
    bool has_disturb = false;

    float thickness;
    float metallic;
    float roughness;

    float ac;
    float diffuse_spd = 10;
    FilmComposition composition = None;
};

class Layer
{
public:
    Layer();
    Layer(int w, int h);
    vector<vector<RustUnit>> units;

    virtual void accept_rules(PipelineConfig*);
    virtual void rust(double);
    virtual vector<vector<RustUnit>> get_units();
};

