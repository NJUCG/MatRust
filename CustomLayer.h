#pragma once
#include "Layer.h"
#include"PipelineConfig.h"
#include<vector>
#include"Perlin.h"
#include"ColorHelper.h"
#include"qdebug.h"
#include"DPD.h"
#include<string>
#include"AlgArgAnalyzer.h"
#include"LayerConfig.h"
using namespace std;

class CustomLayer:public Layer
{
public:
	CustomLayer();
	CustomLayer(string alg_rule, string alg_types);
	CustomLayer(LayerConfig*);
	string alg_type;
	string rule;

	void accept_rules(PipelineConfig*) override;
	void rust(double) override;
protected:
	virtual void set_addition_config(LayerConfig*);

private:

};

