package com.rust.model.layers;

import com.rust.model.FilmComposition;
import com.rust.model.PipelineConfig;
import com.rust.model.PipelineManager;
import com.rust.util.ColorHelper;
import com.rust.util.ImageHelper;
import com.rust.util.noise.Ballistic;
import com.rust.util.noise.DPD;
import glm.vec._4.Vec4;

/**
 * 碱式硫酸铜（蓝绿色） 硫酸铜（蓝色）
 * 受水平程度、曲率影响。
 * 受硫浓度、氧浓度、水分影响
 * 水分将会改变碱式硫酸铜和硫酸铜的比例
 * 纯ballistic 生成的图样离散性太强
 * 利用ballistic不断生成离散点，然后让离散点自行生长
 */
public class SulfideLayer extends BaseLayer{
    //记录屁股地下有多少氧化物
    private float[][] localOutput;
    private float[][] lattice;
    private Ballistic ballistic;
    private DPD dpd;
    public int part_per_sec;
    public int w,h;
    public SulfideLayer(){

    }
    private float curveTwister(float c){
        return (float) Math.pow(c,10);
    }
    @Override
    public void acceptRules(PipelineConfig config) {
        //ballistic = new Ballistic();
        //ballistic.setUp(config.textureWidth,config.textureHeight, (float) (config.sc));
        //ballistic.useLattice = true;
        //localOutput = ballistic.output;
        part_per_sec = 10000 / (int) ((config.textureHeight * config.textureWidth * config.sc * config.oc * config.rh));
        w = config.textureWidth;
        h = config.textureHeight;
        units = new RustUnit[h][w];
        lattice = new float[h][w];
        float maxCurvature = 0;
        for (int i=0;i<h;i++){
            for (int j=0;j<w;j++){
                maxCurvature = Math.max(maxCurvature,config.curvature[i][j]);
            }
        }
        for (int i=0;i<h;i++){
            for (int j=0;j<w;j++){
                lattice[i][j] = config.curvature[i][j] / maxCurvature;
                lattice[i][j] = curveTwister(lattice[i][j]) / 16;
            }
        }
        //ballistic.lattice = lattice;
       // ballistic.useLattice = true;
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                RustUnit unit = new RustUnit();
                unit.thickness = 0;
                unit.composition = FilmComposition.CuSO4;
                unit.color = ColorHelper.colorOf(unit.composition);
                units[i][j] = unit;
            }
        }

        dpd = new DPD();
        System.out.println(part_per_sec);
        dpd.setUp(w,h,part_per_sec, (float) config.sc / 8, lattice,0);
        localOutput = dpd.output;
    }
    private float timer = 0;
    private boolean print = false;
    @Override
    public void rust(double deltaTime) {
        timer += deltaTime;
        //ballistic.rust((int) (part_per_sec * deltaTime));
        dpd.rust((float) deltaTime);
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                units[i][j].thickness = localOutput[i][j];
                units[i][j].roughness = 0.8f;
                units[i][j].metallic = 0.1f;
            }
        }
        if(timer>10 && !print){
            print  = true;
            Vec4[][] ret = new Vec4[h][w];
            for(int i=0;i<h;i++){
                for(int j=0;j<w;j++){
                    float t = units[i][j].thickness * 255;
                    ret[i][j] = new Vec4(t,t,t,255);
                }
            }
            try {
                ImageHelper.saveAsPng(ret,w,h,"src/main/resources/kj.png");
                System.out.println("printed");
            }catch (Exception e){
                System.out.println('a');
            }
        }
    }

    @Override
    public RustUnit[][] getUnits() {
        return units;
    }
}
