package com.rust.util.noise;


/**
 * 周期性用ballistic降下种子，然后以种子和导向图为基础向四周发散
 */
public class EdenGrowWithGuide {
    private float[][] lattice;
    private float seedPerSec;
    private float growSpd;
    private int w,h;
    private Ballistic ballistic = new Ballistic();

    public float[][] output;

    public EdenGrowWithGuide(){
    }
    public void enableLattice(){
        ballistic.useLattice = true;
    }
    public void disableLattice(){
        ballistic.useLattice = false;
    }
    public void setFrame(int w,int h){
        this.w=w;
        this.h=h;
    }
    public void setLattice(float[][] lattice){
        this.lattice = lattice;
    }
    public void setBallisticLattice(float[][] lattice){
        ballistic.lattice = lattice;
    }
    public void setUp(int w,int h,int seedPerSec, float seedGrowSpd,float patternGrowSpd){
        this.w = w;
        this.h = h;
        this.seedPerSec = seedPerSec;
        ballistic.setUp(w,h,seedGrowSpd);
        growSpd = patternGrowSpd;
        output = new float[h][w];
    }
    public void grow(float deltaTime){
        ballistic.rust((int) (seedPerSec * deltaTime));
        
    }

}
