#include "PipelineConfig.h"

PipelineConfig::PipelineConfig()
{
    oc = 0;
    cc = 0;
    sc = 0;
    rh = 0;
    test = 0;
    temperature = 0;
    growTime = 40;
    textureWidth = 800;
    textureHeight = 800;
    generateImage = false;
    diffPath = "diff.png";

    backTexture = vector<vector<vec4>>(textureHeight, vector<vec4>(textureWidth, vec4()));
    metallic = vector<vector<float>>(textureHeight, vector<float>(textureWidth, 0));
    roughness;
    useCurvature = true;
    curvature;
}
