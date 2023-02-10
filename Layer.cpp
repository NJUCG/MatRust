#include "Layer.h"

Layer::Layer()
{
	units = vector<vector<RustUnit>>();
}

Layer::Layer(int w, int h)
{
	units = vector<vector<RustUnit>>(h, vector<RustUnit>(w, RustUnit()));
}

void Layer::accept_rules(PipelineConfig* config)
{
	int w = config->textureWidth;
	int h = config->textureHeight;

	units = vector<vector<RustUnit>>(h, vector<RustUnit>(w, RustUnit()));
}

void Layer::rust(double)
{
}

vector<vector<RustUnit>> Layer::get_units()
{
	return units;
}
