#pragma once
#include "Layer.h"
#include"PipelineConfig.h"
#include<vector>
#include"Perlin.h"
#include"ColorHelper.h"
#include"qdebug.h"
using namespace std;

class OxideLayer: public Layer, public PerlinBackGroundI
{
public:
    float a, b, n, k;
    /**
     * 操作厚度用这个。
     */
    vector<vector<float>> pattern;
    float cutOffHeight = 10;
    float biasHeight = 10;

	OxideLayer();
	void accept_rules(PipelineConfig*) override;
	void rust(double) override;

    float twist(vec2 pos) override;

    vector<vector<RustUnit>> get_units() override;

private:
    float ab, kdn, pe;
    PipelineConfig* config;
    Perlin perlin;
    double currentTime = 0;
    float growth = 0;

    bool shown = false;

    float logRustSpd(float t);
    float powerRustSpd(float t);
};

