package com.rust.model.layers;

import com.rust.model.PipelineConfig;
import com.rust.util.GlmExtension;
import com.rust.util.ImageHelper;
import com.rust.util.noise.Perlin;
import com.rust.util.noise.PerlinBackGroundI;
import glm.vec._2.Vec2;
import glm.vec._4.Vec4;

public class TestLayer extends BaseLayer implements PerlinBackGroundI {
    private double currentTime = 0;
    private Perlin perlin;
    @Override
    public void acceptRules(PipelineConfig config) {
        super.acceptRules(config);
        perlin = new Perlin();
        perlin.setUp(config.textureWidth,config.textureHeight,40,this);
        float[][] pattern = perlin.perlinPattern();
        Vec4[][] colors = new Vec4[config.textureHeight][config.textureWidth];
        for (int i =0 ;i<h;i++){
            for (int j=0;j<w;j++) {
                int t = (int) Math.abs(pattern[i][j] * 255);
                colors[i][j] = new Vec4(t, t, t, 255);
            }
        }
        try{
            ImageHelper.saveAsPng(colors, config.textureWidth, config.textureHeight, "src/main/resources/test.png");
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    @Override
    public void rust(double deltaTime) {
        currentTime += deltaTime;
        float[][] pattern = perlin.perlinPattern();
        for (int i =0 ;i<h;i++){
            for (int j=0;j<w;j++){
                int t = (int) Math.abs(pattern[i][j] * 255);
                RustUnit unit = new RustUnit();
                unit.color = new Vec4(t,t,t,255);
                unit.thickness = 1;
                units[i][j] = unit;
            }
        }
    }

    @Override
    public float twist(Vec2 pos, Perlin perlin) {
        /*float res = turb(pos, 7,perlin) / 2;
        res = (float) (0.5 * (1 + sin(2 * res * PI))* 255);
        res -= (20 - currentTime);
        if (res < 0) {
            res = 0;
        }*/
        return perlin.perlin_noise(pos);
    }
    private float turb(Vec2 p,int depth,Perlin perlin) {
        float accum = 0;
        Vec2 temp_p = p;
        for (int i = 0; i < depth; i++) {
            accum += perlin.perlin_noise(temp_p);
            temp_p = GlmExtension.mul(temp_p,2);
        }
        return accum;
    }
}
