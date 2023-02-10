#pragma once
#include<vector>
#include<string>
#include"ImageHelper.h"
using namespace std;

class CurvatureHelper
{
public:
    static float sigma;
    static float _2squreSigmaDe;
    static float _2squreSigmaPIDe;
    // 直接获取曲率图
    static vector<vector<float>> generate(string path, int comps = 3) {
        vector<vector<vec4>> img;
        int nrComponents = 0;
        ImageHelper::load_pic(path, img, nrComponents);
        if (img.size() == 0) {
            return vector<vector<float>>();
        }
        int w = img[0].size();
        int h = img.size();
        vector<vector<float>> res(h, vector<float>(w, 0.0f));
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int b = img[i][j].b;
                res[i][j] = 255 - std::abs(b);
            }
        }
        return res;
    }

    static float gs(int dx, int dy) {
        float exp = -(dx * dx + dy * dy) * _2squreSigmaDe;
        return (float)(_2squreSigmaPIDe * std::exp(exp));
    }
};

