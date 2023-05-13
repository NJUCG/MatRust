#pragma once
#include"ImageHelper.h"
#include"EventAdapter.h"
#include"Perlin.h"
#include"MyRandom.h"

class NormalDisturbHelper: public PerlinBackGroundI {
public:
    NormalDisturbHelper(int w, int h, int wavelength) {
        this->r_perlin = new Perlin();
        this->g_perlin = new Perlin();
        this->b_perlin = new Perlin();
        this->w = w;
        this->h = h;
        this->r_perlin->setUp(w, h, wavelength, this);
        this->g_perlin->setUp(w, h, wavelength, this);
        this->b_perlin->setUp(w, h, wavelength, this);
        this->r_set_up = this->g_set_up = this->b_set_up = false;
    }
    ~NormalDisturbHelper() {
        if (r_perlin) {
            delete r_perlin;
        }
        if (g_perlin) {
            delete g_perlin;
        }
        if (b_perlin) {
            delete b_perlin;
        }
    }
    float twist(vec2 pos) override{
        float n;
        if (r_set_up) {
            n = r_perlin->perlin_noise(pos);
        }
        else if (g_set_up) {
            n = g_perlin->perlin_noise(pos);
        }
        else if (b_set_up) {
            n = b_perlin->perlin_noise(pos);
        }

        //n = (float)sin(M_PI / 4 + n);
        return n;// (float)(0.75 + abs(n) * 0.25);
    }
    void generate_pattern(QString path) {
        if (!r_perlin || !g_perlin || !b_perlin) {
            return;
        }
        unsigned char* pattern = new unsigned char[h * w * 3];
        long long int idx = 0;
        r_set_up = true;
        vector<vector<float>> r_pattern = r_perlin->perlinPattern();
        r_set_up = false;
        g_set_up = true;
        vector<vector<float>> g_pattern = g_perlin->perlinPattern();
        g_set_up = false;
        b_set_up = true;
        vector<vector<float>> b_pattern = b_perlin->perlinPattern();
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                pattern[idx++] = r_pattern[i][j] * 255;//MyRandom::_0_1F() * 255;
                pattern[idx++] = g_pattern[i][j] * 255;//MyRandom::_0_1F() * 255;
                pattern[idx++] = b_pattern[i][j] * 255;//MyRandom::_0_1F() * 255;
            }
        }
        ImageHelper::save_pic(path.toStdString(), pattern, 3, w, h);
    }
private:
    Perlin* perlin = nullptr;
    Perlin* r_perlin = nullptr;
    Perlin* g_perlin = nullptr;
    Perlin* b_perlin = nullptr;
    long long int w;
    long long int h;
    bool r_set_up, g_set_up, b_set_up;
};