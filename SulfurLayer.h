#pragma once
#include"Layer.h"
#include"DPD.h"

class SulfurLayer: public Layer
{
public:
    int part_per_sec;
    int w, h;
	SulfurLayer();
	void accept_rules(PipelineConfig*) override;
	void rust(double) override;
private:
	vector<vector<float>> localOutput;
	vector<vector<float>> lattice;
	DPD dpd;
	float timer = 0;
	bool print = false;
	bool prepared = false;
	float curveTwister(float c);
};

