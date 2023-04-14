#pragma once
#include"PerlinBackGroundI.h"
#include<vector>
#include <QtCore/qmath.h>
using namespace std;

class Perlin
{
public:
    int width;
    int height;
    int blockWidth;
    PerlinBackGroundI* twister;
    Perlin();
    void setUp(int width, int height, int blockWidth, PerlinBackGroundI* twister) {
        this->width = width;
        this->height = height;
        this->blockWidth = blockWidth;
        this->twister = twister;
        perlinInit();
    }
    vector<vector<float>> perlinPattern() {
        vector<vector<float>> res = vector<vector<float>>(height, vector<float>(width, 0));
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                vec2 pos;
                pos.x = (float)j / (float)blockWidth;
                pos.y = (float)i / (float)blockWidth;
                float t = twister->twist(pos);
                res[i][j] = t;
            }
        }
        return res;
    }
    float perlin_noise(vec2 pos) {
        // decimal of pos
        float xd = (float)(pos.x - floor(pos.x));
        float yd = (float)(pos.y - floor(pos.y));
        // integer of pos
        int xi = (int)floor(pos.x);
        int yi = (int)floor(pos.y);

        vec2 grad_bottomLeft = grad(xi, yi);
        vec2 delta_bottomLeft = vec2(xd, yd);
        vec2 grad_bottomRight = grad(xi + 1, yi);
        vec2 delta_bottomRight = vec2(xd - 1, yd);
        vec2 grad_topLeft = grad(xi, yi + 1);
        vec2 delta_topLeft = vec2(xd, yd - 1);
        vec2 grad_topRight = grad(xi + 1, yi + 1);
        vec2 delta_topRight = vec2(xd - 1, yd - 1);

        float xf = twister->fade(xd);
        float yf = twister->fade(yd);

        float p0p1 = (1 - xf) * dot(grad_bottomLeft, delta_bottomLeft) + xf * dot(grad_bottomRight, delta_bottomRight);
        float p2p3 = (1 - xf) * dot(grad_topLeft, delta_topLeft) + xf * dot(grad_topRight, delta_topRight);

        float v = (1 - yf) * p0p1 + yf * p2p3;
        return v;
    }

private:
    /*
    perlin需要的变量
     */
    vector<int> P;
    vector<vec2> G;
    void perlinInit() {
        P = vector<int>(256);
        G = vector<vec2>(256);
        for (int i = 0; i < 256; i++) {
            P[i] = i;
        }
        for (int i = 0; i < 256; i++) {
            int t = (int)(rand()%255 + 1);
            int p = P[t];
            P[t] = P[i];
            P[i] = p;
        }
        for (int i = 0; i < 256; i++) {
            double x, y, theta;
            double t = (float)(rand())/(RAND_MAX + 1.0f);
            theta = 2 * t * M_PI;
            x = cos(theta);
            y = sin(theta);
            G[i] = vec2(x, y);
        }
    }
    vec2 grad(int x, int y) {
        return G[P[P[x & 0xff] + y & 0xff]];
    }
};

