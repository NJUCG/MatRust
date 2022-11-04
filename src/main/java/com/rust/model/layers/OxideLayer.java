package com.rust.model.layers;

import com.rust.model.FilmComposition;
import com.rust.model.PipelineConfig;
import com.rust.util.ColorHelper;
import com.rust.util.noise.Perlin;
import com.rust.util.noise.PerlinBackGroundI;
import glm.vec._2.Vec2;
import glm.vec._4.Vec4;

/**
 * 氧化层
 * 温度指数性地影响氧化层演进速度
 * Cu2O -> 并存 -> CuO
 * 温度区间为 0～100
 * 0~60 使用对数增厚 x = aln(bt+1), spd = ab/(bt+1)
 * 60~100 使用指数增厚 x^n = kt, spd = (k/n)(kt)^((1-n)/n)
 * 更高的温度没有意义，更低的温度将模拟0～60的公式
 * a,b,n,k 由温度、rh决定
 * particle_per_sec 由氧浓度和面积决定
 * 其他成员用于加速计算,ps为particle_size
 * ballistic 效果不太好，换成perlin
 */
public class OxideLayer extends BaseLayer implements PerlinBackGroundI {
    public float a,b,n,k;
    private float ab,kdn,pe;
    private PipelineConfig config;
    private double currentTime = 0;
    private RustUnit[][] rustUnits;
    /**
     * 操作厚度用这个。
     */
    public float[][] pattern;
    public float cutOffHeight = 10;
    public float biasHeight = 10;
    @Override
    public void acceptRules(PipelineConfig config) {
        super.acceptRules(config);
        this.config = config;
        a = (float) (config.temperature / 75 + config.rh * 10) / 5;
        b = a * 15;
        k = a / 15;
        n=2;
        ab = a*b;
        kdn = k/n;
        pe = (1-n)/n;
        Perlin perlin = new Perlin();
        perlin.setUp(config.textureWidth,config.textureHeight,30,this);
        pattern = perlin.perlinPattern();
        rustUnits = new RustUnit[config.textureHeight][config.textureWidth];
        for(int i=0;i<config.textureHeight;i++){
            for(int j=0;j<config.textureWidth;j++){
                RustUnit initUnit = new RustUnit();
                initUnit.color = ColorHelper.colorOf(FilmComposition.CuII_O);
                initUnit.composition = FilmComposition.CuI_O;
                pattern[i][j] = -cutOffHeight + pattern[i][j] * biasHeight;
                rustUnits[i][j] = initUnit;
            }
        }
    }
    float growth = 0;
    @Override
    public void rust(double deltaTime) {
        currentTime+=deltaTime;
        if(config.temperature<60){
            growth = (float) (logRustSpd((float) currentTime) * deltaTime) ;
        }else {
            growth = (float) (powerRustSpd((float) currentTime)*deltaTime) ;
        }
        for(int i=0;i<config.textureHeight;i++){
            for(int j=0;j<config.textureWidth;j++){
                pattern[i][j] += growth;
                rustUnits[i][j].thickness = Math.min(biasHeight,Math.max(0,pattern[i][j]));
                if(rustUnits[i][j].thickness>0){
                    rustUnits[i][j].roughness = 0.2f;
                    rustUnits[i][j].metallic = 0.1f;
                }
            }
        }
    }

    @Override
    public RustUnit[][] getUnits() {
        return rustUnits;
    }

    private float logRustSpd(float t){
        return ab/(b*t+1);
    }
    private float powerRustSpd(float t){
        return (float) (kdn*Math.pow(k*t,pe));
    }
    private float logThickness(float t){
        return (float) (a* Math.log(b*t+1));
    }
    private float powerThickness(float t){
        return (float) Math.pow(k*t,1/n);
    }
    @Override
    public float twist(Vec2 pos, Perlin perlin) {
        float n = perlin.perlin_noise(pos);
        n = (float) Math.sin(Math.PI / 4 + n/4);
        return (float) (0.75 + Math.abs(n) * 0.25);
    }
}
