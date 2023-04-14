#pragma once
#include"CustomLayer.h"
#include<vector>
#include"Perlin.h"
#include"ColorHelper.h"
#include"qdebug.h"

class CustomPerlinLayer : public CustomLayer, public PerlinBackGroundI
{
public:
    float a, b, n, k;
    /**
     * 操作厚度用这个。
     */
    vector<vector<float>> pattern;
    float cutOffHeight = 10;
    float biasHeight = 10;

	CustomPerlinLayer(string rule);
    CustomPerlinLayer(LayerConfig*);

    void accept_rules(PipelineConfig*) override;
    void rust(double) override;
    float twist(vec2 pos) override;

    vector<vector<RustUnit>> get_units() override;

protected:
    float ab, kdn, pe;
    
    PipelineConfig* config;
    LayerConfig* addition_config;

    Perlin perlin;
    double currentTime = 0;
    float growth = 0;

    bool shown = false;

    float logRustSpd(float t);
    float powerRustSpd(float t);

    void set_addition_config(LayerConfig*) override;
};

