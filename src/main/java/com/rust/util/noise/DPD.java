package com.rust.util.noise;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class DPD {
    public int w,h;
    public float growSpd;
    public float[][] output;
    public float[][] lattice;
    public int seedGate;
    private final int[] dx = {1, 1,  0, -1, -1,-1,0};
    private final int[] dy = {0,-1, -1, -1, 0,1,1};
    private final List<DPDPatch> patches = new ArrayList<>();
    public int maxPatch = 8;
    public int initSeed = 2;
    public DPD(){

    }
    private float latticeMax=0;
    private float[][] latticePrefix;
    private void addSeed(){
        float t = (float) (Math.random() * latticeMax);
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                if(t<latticePrefix[i][j]){
                    DPDPatch patch = new DPDPatch();
                    patch.outlineX.add(j);
                    patch.outlineY.add(i);
                    output[i][j] = 0.001f;
                    patches.add(patch);
                    return;
                }
            }
        }
    }
    // 纯随机效果可能不太好，添加一些伪随机
    private void addIntendedSeed(){
        float t = (float) (Math.random() * latticeMax);
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                if(t<latticePrefix[i][j]){
                    DPDPatch patch = new DPDPatch();
                    patch.outlineX.add(j);
                    patch.outlineY.add(i);
                    output[i][j] = 0.001f;
                    patches.add(patch);
                    return;
                }
            }
        }
    }
    public void setUp(int w,int h,int seedTimer,float growSpd,float[][] lattice,int initSeed){
        this.lattice = lattice;
        this.growSpd = growSpd;
        this.w = w;
        this.h = h;
        this.initSeed = initSeed;
        this.seedTimer = seedTimer;
        this.seedGate = seedTimer;
        output = new float[h][w];
        for (int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                output[i][j] = 0;
            }
        }
        latticePrefix = new float[h][w];
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                latticeMax+=lattice[i][j];
                latticePrefix[i][j] = latticeMax;
            }
        }

        // 制作初始种子
        for(int i=0;i<initSeed;i++){
            addSeed();
        }
    }
    private float seedTimer = 0;
    public void rust(float deltaTime){
        if(patches.size()<maxPatch){
            seedTimer -= deltaTime;
            if(seedTimer<0){
                seedTimer = seedGate;
                addSeed();
            }
        }
        Set<Integer> toRemove = new HashSet<>();
        // grow已有图像
        for (DPDPatch patch : patches) {
            int os = patch.outlineX.size();
            for (int j = 0; j < os; j++) {
                int x = patch.outlineX.get(j);
                int y = patch.outlineY.get(j);
                int nearByCount = 0;
                for (int k = 0; k < 7; k++) {
                    int nx = x + dx[k];
                    int ny = y + dy[k];
                    if (nx >= 0 && ny >= 0 && nx < w && ny < h) {
                        if (output[ny][nx] > 0) {
                            nearByCount++;
                        } else if (output[ny][nx] <= 0 && Math.random() < lattice[ny][nx]) {
                            nearByCount++;
                            output[ny][nx] = 0.001f;
                            patch.outlineX.add(nx);
                            patch.outlineY.add(ny);
                        } else {
                            nearByCount--;
                        }
                    }else{
                        nearByCount++;
                    }
                }
                if (nearByCount == 4) {
                    toRemove.add(j);
                }
            }
            for (Integer index : toRemove
            ) {
                patch.outlineX.remove(index);
                patch.outlineX.remove(index);
            }
            toRemove.clear();
        }
        for (int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                if(output[i][j]>0){
                    output[i][j] += growSpd * deltaTime;
                }
            }
        }
    }
}
