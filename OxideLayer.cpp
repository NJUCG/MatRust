#include "OxideLayer.h"

OxideLayer::OxideLayer()
{
		
}

void OxideLayer::accept_rules(PipelineConfig* config)
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
	perlin = Perlin();
	perlin.setUp(config->textureWidth, config->textureHeight, 30, this);
	pattern = perlin.perlinPattern();

	unsigned char* p_char = new unsigned char[config->textureHeight * config->textureWidth * 3];
	int idx = 0;
	for (int i = 0; i < config->textureHeight; i++) {
		for (int j = 0; j < config->textureWidth; j++) {
			p_char[idx++] = (unsigned char)(pattern[i][j] * 255);
			p_char[idx++] = (unsigned char)(pattern[i][j] * 255);
			p_char[idx++] = (unsigned char)(pattern[i][j] * 255);
		}
	}

	ImageHelper::save_pic("perlin.png", p_char, 3, config->textureWidth, config->textureHeight);

	for (int i = 0; i < config->textureHeight; i++) {
		for (int j = 0; j < config->textureWidth; j++)
		{
			RustUnit& unit = units[i][j];
			unit.color = ColorHelper::colorOf(FilmComposition::CuI_O);
			unit.thickness = 0;
			unit.composition = FilmComposition::CuI_O;
			pattern[i][j] = -cutOffHeight + pattern[i][j] * biasHeight;
		}
	}
}


void OxideLayer::rust(double deltaTime)
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
				units[i][j].roughness = 0.2f;
				units[i][j].metallic = 0.1f;
			}
		}
	}
}
float OxideLayer::twist(vec2 pos)
{
	float n = perlin.perlin_noise(pos);
	n = (float)sin(M_PI / 4 + n / 4);
	return (float)(0.75 + abs(n) * 0.25);
}
vector<vector<RustUnit>> OxideLayer::get_units()
{
	return units;
}
float OxideLayer::logRustSpd(float t) {
	return ab / (b * t + 1);
}
float OxideLayer::powerRustSpd(float t) {
	return (float)(kdn * pow(k * t, pe));
}