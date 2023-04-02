#include "SulfurLayer.h"

SulfurLayer::SulfurLayer()
{
}

void SulfurLayer::accept_rules(PipelineConfig* config)
{
    Layer::accept_rules(config);
    part_per_sec = 200 * (int)((config->textureHeight * config->textureWidth * (config->sc+0.01f) * (config->oc+0.01f) * (config->rh + 0.01f)));
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
            unit.composition = FilmComposition::CuSO4;
            unit.color = ColorHelper::colorOf(unit.composition);
            units[i][j] = unit;
        }
    }

    dpd = DPD();
    dpd.setUp(w, h, part_per_sec, (float)config->sc / 1.2f, lattice, 4);
    localOutput = dpd.output;
    prepared = true;
}

void SulfurLayer::rust(double delta)
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

float SulfurLayer::curveTwister(float c) {
    return pow(c, 10);
}
