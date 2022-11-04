package com.rust.util.noise;

import com.rust.util.GlmExtension;
import glm.vec._2.Vec2;

import static java.lang.Math.cos;
import static java.lang.Math.sin;

public class Perlin {
    public int width;
    public int height;
    public int blockWidth;
    public PerlinBackGroundI twister;
    public Perlin(){

    }
    public void setUp(int width,int height,int blockWidth, PerlinBackGroundI twister){
        this.width=width;
        this.height=height;
        this.blockWidth=blockWidth;
        this.twister=twister;
        perlinInit();
    }
    public float[][] perlinPattern(){
        float[][] res = new float[height][width];
        for(int i=0;i<height;i++){
            for (int j=0;j<width;j++){
                Vec2 pos = new Vec2();
                pos.x = (float)j/(float) blockWidth;
                pos.y = (float)i/(float) blockWidth;
                float t = twister.twist(pos,this);
                res[i][j] = t;
            }
        }
        return res;
    }

    /*
    perlin需要的变量
     */
    int[] P = new int[256];
    Vec2[] G = new Vec2[256];
    private void perlinInit(){
        for (int i = 0; i < 256; i++) {
            P[i] = i;
        }
        for (int i = 0; i < 256; i++) {
            int t = (int) (Math.random() * 256);
            int p= P[t];
            P[t] = P[i];
            P[i] = p;
        }
        for (int i=0;i<256;i++){
            double x, y, theta;
            double t = (float) Math.random();
            theta = 2 * t * Math.PI;
            x = cos(theta);
            y = sin(theta);
            G[i] = new Vec2(x,y);
        }
    }
    public float perlin_noise(Vec2 pos){
        // decimal of pos
        float xd = (float) (pos.x - Math.floor(pos.x));
        float yd = (float) (pos.y - Math.floor(pos.y));
        // integer of pos
        int xi = (int) Math.floor(pos.x);
        int yi = (int) Math.floor(pos.y);

        Vec2 grad_bottomLeft = grad(xi,yi);
        Vec2 delta_bottomLeft = new Vec2(xd,yd);
        Vec2 grad_bottomRight = grad(xi+1,yi);
        Vec2 delta_bottomRight = new Vec2(xd-1,yd);
        Vec2 grad_topLeft = grad(xi,yi+1);
        Vec2 delta_topLeft = new Vec2(xd,yd-1);
        Vec2 grad_topRight = grad(xi+1,yi+1);
        Vec2 delta_topRight = new Vec2(xd-1,yd-1);

        float xf = twister.fade(xd);
        float yf = twister.fade(yd);

        float p0p1 = (1 - xf) * GlmExtension.dot(grad_bottomLeft, delta_bottomLeft) + xf * GlmExtension.dot(grad_bottomRight, delta_bottomRight);
        float p2p3 = (1 - xf) * GlmExtension.dot(grad_topLeft, delta_topLeft) + xf * GlmExtension.dot(grad_topRight, delta_topRight);

        float v = (1 - yf) * p0p1 + yf * p2p3;
        return v;
    }

    private Vec2 grad(int x,int y){
       return G[P[P[x & 0xff] + y & 0xff]];
    }
}
