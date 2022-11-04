package com.rust.util.noise;

import glm.vec._2.i.Vec2i;

public class Ballistic {
    public float[][] output;
    public float[][] lattice = null;
    public int width,height;
    public float growSpd;
    public boolean useLattice = false;
    public Ballistic(){

    }
    public void setUp(int width,int height,float growSpd){
        this.width = width;
        this.height = height;
        this.growSpd = growSpd;
        output = new float[height][width];
    }
    public void setUp(int width,int height,float growSpd,float[][] lattice){
        this.width = width;
        this.height = height;
        this.lattice = lattice;
        this.growSpd = growSpd;
        output = new float[height][width];
    }
    public void rust(int n){
        for(int i=0;i<n;i++) {
            int x = (int) (Math.random() * width);
            int y = (int) (Math.random() * height);
            if (!useLattice) {
                addPart(x, y);
            } else {
                double gate = Math.random();
                if (gate < lattice[y][x])
                    addPart(x, y);
            }
        }
    }
    public void refresh(){
        output = new float[height][width];
    }
    private void addPart(int x,int y){
        if(output[y][x] > 0){
            output[y][x] += growSpd;
        }
        float thickness = growSpd;
        for(int i=-1;i<=1;i++){
            for(int j=-1;j<=1;j++){
                int nx = x+i;
                int ny = y+j;
                if (nx >= 0 && nx < width && ny >= 0 && ny < height && thickness < output[ny][nx]) {
                    thickness = output[ny][nx];
                }
            }
        }
        output[y][x] = thickness;
     }

}
