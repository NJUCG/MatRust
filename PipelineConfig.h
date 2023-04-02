#pragma once
#include<string>
#include<glm/glm.hpp>
#include<vector>
#include"LayerConfig.h"
using namespace std;
using namespace glm;

class PipelineConfig
{
public:
	PipelineConfig();
    double oc;
    double cc;
    double sc;
    double rh;
    double test;
    double temperature;
    double growTime = 40;
    int textureWidth = 400;
    int textureHeight = 400;
    bool generateImage = false;
    string diffPath = "diff.png";
    
    vector<vector<vec4>> backTexture;
    vector<vector<float>> metallic;
    vector<vector<float>> roughness;
    bool useCurvature = true;
    vector<vector<float>> curvature;

    vector<LayerConfig*> layers;
};

