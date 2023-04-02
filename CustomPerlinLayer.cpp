#include "CustomPerlinLayer.h"

CustomPerlinLayer::CustomPerlinLayer(string rule) : CustomLayer(rule, "Perlin")
{
}

CustomPerlinLayer::CustomPerlinLayer(LayerConfig* config): CustomLayer(config)
{
	set_addition_config(config);
}

void CustomPerlinLayer::set_addition_config(LayerConfig* addition_config)
{
	CustomLayer::set_addition_config(addition_config);
	this->addition_config = addition_config;
	this->biasHeight = *((float*)this->addition_config->addition_properties["bias height"]);
	this->cutOffHeight = *((float*)this->addition_config->addition_properties["cutoff height"]);
}

void CustomPerlinLayer::accept_rules(PipelineConfig* config)
{
	Layer::accept_rules(config);
	this->config = config;

	a = (float)(config->temperature / 45 + config->rh * 20) / 4;
	b = a * 15;
	k = a / 15;
	n = 2;
	ab = a * b;
	kdn = k / n;
	pe = (1 - n) / n;

	perlin.setUp(config->textureWidth, config->textureHeight, *((float*)this->addition_config->addition_properties["block width"]), this);
	pattern = perlin.perlinPattern();

	for (int i = 0; i < config->textureHeight; i++) {
		for (int j = 0; j < config->textureWidth; j++)
		{
			RustUnit& unit = units[i][j];
			unit.color = addition_config->color;
			unit.thickness = 0;
			unit.composition = FilmComposition::Custom;
			unit.ac = addition_config->ac;
			unit.roughness = addition_config->roughness;
			unit.metallic = addition_config->metallic;
			pattern[i][j] = -cutOffHeight + pattern[i][j] * biasHeight;
		}
	}
}

void CustomPerlinLayer::rust(double deltaTime)
{
	currentTime += deltaTime;

	if (config->temperature < 60) {
		growth = (float)(logRustSpd((float)currentTime) * deltaTime);
	}
	else {
		growth = (float)(powerRustSpd((float)currentTime) * deltaTime);
	}

	for (int i = 0; i < config->textureHeight; i++) {
		for (int j = 0; j < config->textureWidth; j++) {
			pattern[i][j] += growth;
			units[i][j].thickness = std::min(biasHeight, std::max(0.0f, pattern[i][j]));
			if (units[i][j].thickness > 0) {
				units[i][j].roughness = addition_config->roughness;
				units[i][j].metallic = addition_config->metallic;
			}
		}
	}
}

float CustomPerlinLayer::twist(vec2 pos)
{
	float n = perlin.perlin_noise(pos);
	n = (float)sin(M_PI / 4 + n / 4);
	return (float)(0.75 + abs(n) * 0.25);
}

vector<vector<RustUnit>> CustomPerlinLayer::get_units()
{
	return units;
}

float CustomPerlinLayer::logRustSpd(float t)
{
	return ab / (b * t + 1);
}

float CustomPerlinLayer::powerRustSpd(float t)
{
	return (float)(kdn * pow(k * t, pe));
}
