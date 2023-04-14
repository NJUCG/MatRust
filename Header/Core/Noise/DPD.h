#pragma once
#include<vector>
#include<set>
#include "DPDPatch.h"
#include<qdebug.h>
using namespace std;

class DPD {
public:
    DPD() {

    };
    static float rand_() {
        return (float)(rand()) / (RAND_MAX + 1.0f);
    };
    int w, h;
    float growSpd;
    vector<vector<float>> output;
    vector<vector<float>> lattice;
    int seedGate;
    vector<int> dx = { 1, 1,  0, -1, -1,-1,0,1 };
    vector<int> dy = { 0,-1, -1, -1, 0,1,1,1 };
    vector<DPDPatch*> patches;
    int maxPatch = 1000;
    int initSeed = 2;
    float latticeMax = 0;
    vector<vector<float>> latticePrefix;
    void addSeed() {
        float t = (float)(rand_() * latticeMax);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (t < latticePrefix[i][j]) {
                    DPDPatch* patch = new DPDPatch();
                    patch->outlineX.push_back(j);
                    patch->outlineY.push_back(i);
                    output[i][j] = 0.001f;
                    patches.push_back(patch);
                    return;
                }
            }
        }
    }
    // 纯随机效果可能不太好，添加一些伪随机
    void addIntendedSeed() {
        float t = (float)(rand_() * latticeMax);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (t < latticePrefix[i][j]) {
                    DPDPatch* patch = new DPDPatch();
                    patch->outlineX.push_back(j);
                    patch->outlineY.push_back(i);
                    output[i][j] = 0.001f;
                    patches.push_back(patch);
                    return;
                }
            }
        }
    }
    void setUp(int w, int h, float seedTimer, float growSpd, vector<vector<float>> lattice, int initSeed) {
        this->lattice = lattice;
        this->growSpd = growSpd;
        this->w = w;
        this->h = h;
        this->initSeed = initSeed;
        this->seedTimer = seedTimer;
        this->seedGate = seedTimer;
        output = vector<vector<float>>(h, vector<float>(w, 0));
        latticePrefix = vector<vector<float>>(h, vector<float>(w, 0));
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                latticeMax += lattice[i][j];
                latticePrefix[i][j] = latticeMax;
            }
        }
        // 制作初始种子
        for (int i = 0; i < initSeed; i++) {
            addSeed();
        }
    }
    float seedTimer = 0;
    void rust(float deltaTime) {
        if (patches.size() < maxPatch) {
            seedTimer -= deltaTime;
            if (seedTimer < 0) {
                seedTimer = seedGate;
                addSeed();
            }
        }
        set<int> toRemove;
        // grow已有图像
        int pl = patches.size();
        for (int i = 0; i < pl;i++) {
            DPDPatch* patch = patches[i];
            int os = patch->outlineX.size();
            for (int j = 0; j < os; j++) {
                int x = patch->outlineX[j];
                int y = patch->outlineY[j];
                int nearByCount = 0;
                for (int k = 0; k < 8; k++) {
                    int nx = (x + dx[k] + w) % w;
                    int ny = (y + dy[k] + h) % h;
                    //qDebug() << x <<"," << dx[k] <<","<<y<<","<<dy[k]<<","<<ny << "," << nx << "," << h << "," << w;
                    if (output[ny][nx] > 0) {
                        nearByCount++;
                    }
                    else if (output[ny][nx] <= 0 && rand_() < lattice[ny][nx]) {
                        nearByCount++;
                        output[ny][nx] = 0.001f;
                        patch->outlineX.push_back(nx);
                        patch->outlineY.push_back(ny);
                    }
                }
                if (nearByCount == 8) {
                    toRemove.insert(j);
                }
            }
            for (set<int>::iterator index = toRemove.begin(); index != toRemove.end();index++){
                int idx = *index;
                vector<int> outline_x = patch->outlineX;
                vector<int> outline_y = patch->outlineY;
                outline_x.erase(outline_x.begin() + idx);
                outline_y.erase(outline_y.begin() + idx);
            }
            toRemove.clear();
        }
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (output[i][j] > 0) {
                    output[i][j] += growSpd * deltaTime;
                }
            }
        }
    }
};
