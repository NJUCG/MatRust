#include "CustomDPDLayer.h"

CustomDPDLayer::CustomDPDLayer(LayerConfig* config)
{
	addition_config = config;
}

void CustomDPDLayer::accept_rules(PipelineConfig* p_config)
{
	Layer::accept_rules(p_config);
	this->config = p_config;

	string part_per_sec_rule = ((QString*)addition_config->addition_properties["part per sec"])->toStdString();

	AlgArgAnalyzer::analyzer->register_config(p_config);
    float key = AlgArgAnalyzer::analyzer->get_arg(part_per_sec_rule);
	part_per_sec = AlgArgAnalyzer::analyzer->get_arg(part_per_sec_rule) / (config->textureHeight * config->textureWidth);

    string grow_spd_rule = ((QString*)addition_config->addition_properties["grow spd"])->toStdString();
    
    float grow_spd = AlgArgAnalyzer::analyzer->analyzer->get_arg(grow_spd_rule);

    w = config->textureWidth;
    h = config->textureHeight;
    lattice = vector<vector<float>>(h, vector<float>(w, 0));

    float maxCurvature = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            maxCurvature = std::max(maxCurvature, config->curvature[i][j]);
        }
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            lattice[i][j] = config->curvature[i][j] / maxCurvature;
            lattice[i][j] = curveTwister(lattice[i][j]) / 16;
        }
    }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            RustUnit unit;
            unit.thickness = 0;
            unit.ac = addition_config->ac;
            unit.composition = FilmComposition::Custom;
            unit.color = addition_config->color;
            unit.roughness = addition_config->roughness;
            unit.metallic = addition_config->metallic;
            units[i][j] = unit;
        }
    }

    dpd = DPD();
    float initc = *((float*)addition_config->addition_properties["init seed"]);

    dpd.setUp(w, h, part_per_sec, grow_spd, lattice, initc);
    localOutput = dpd.output;
    prepared = true;
}

void CustomDPDLayer::rust(double delta)
{
    timer += delta;
    if (!prepared) {
        return;
    }
    dpd.rust((float)delta);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            units[i][j].thickness = dpd.output[i][j];
            units[i][j].roughness = 0.8f;
            units[i][j].metallic = 0.1f;
            if (units[i][j].thickness > 0) {
                int k = 0;
            }
        }
    }
}

float CustomDPDLayer::curveTwister(float c)
{
    return pow(c, 10);
}
