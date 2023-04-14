#pragma once
#include"CustomLayer.h"
#include<vector>
#include"DPD.h"
#include"ColorHelper.h"
#include"qdebug.h"
#include<string>
#include"MyRandom.h"
using namespace std;

class CustomDPDLayer :public CustomLayer
{
public:
	float part_per_sec;
	int w, h;
	CustomDPDLayer(LayerConfig*);
	void accept_rules(PipelineConfig*) override;
	void rust(double) override;
protected:
	PipelineConfig* config;
	LayerConfig* addition_config;

	vector<vector<float>> localOutput;
	vector<vector<float>> lattice;
	DPD dpd;
	float timer = 0;
	bool print = false;
	bool prepared = false;
	float curveTwister(float c);
};

