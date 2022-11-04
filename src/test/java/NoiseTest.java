import com.rust.util.GlmExtension;
import com.rust.util.ImageHelper;
import com.rust.util.noise.Ballistic;
import com.rust.util.noise.DPD;
import com.rust.util.noise.Perlin;
import com.rust.util.noise.PerlinBackGroundI;
import glm.vec._2.Vec2;
import glm.vec._4.Vec4;
import jglm.Mat;

public class NoiseTest implements PerlinBackGroundI {
    public static double total = 1;
    public int w = 200;
    public int h = 200;
    @Override
    public float twist(Vec2 pos, Perlin perlin) {
        float n = perlin.perlin_noise(pos);
        n = (float) Math.sin(Math.PI / 4 + n/4);
        return (float) (0.5 + Math.abs(n)/2);
    }

    public void dpd_test(){
        int width,height;
        width = 200;
        height = 200;
        float[][] lattice = new float[height][width];
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                if(i<height/2){
                    lattice[i][j] = 0.2f;
                }else{
                    lattice[i][j] = 0.8f;
                }
            }
        }
        DPD dpd = new DPD();
        dpd.setUp(width,height,2,0.2f,lattice,2);
        for(int i=0;i<100;i++){
            dpd.rust(0.02f);
        }
        Vec4[][] colors = new Vec4[height][width];
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                float t = dpd.output[i][j] * 255;
                if(t>255){
                    t=255;
                }
                colors[i][j] = new Vec4(t,t,t,255);
            }
        }
        try {
            ImageHelper.saveAsPng(colors, width, height, "src/main/resources/dpd.png");
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public void ballistic_test(){
        int width,height;
        width = 200;
        height = 200;
        float[][] lattice = new float[height][width];
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                if(i<height/2){
                    lattice[i][j] = 0.3f;
                }else{
                    lattice[i][j] = 0.9f;
                }
            }
        }
        Ballistic ballistic = new Ballistic();
        ballistic.setUp(width,height,60);
        ballistic.lattice = lattice;
        ballistic.useLattice = true;
        ballistic.rust(15000);
        Vec4[][] colors = new Vec4[height][width];
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                float t = ballistic.output[i][j];
                if(t>255){
                    t=255;
                }
                colors[i][j] = new Vec4(t,t,t,255);
            }
        }
        try {
            ImageHelper.saveAsPng(colors, width, height, "src/main/resources/ballistic.png");
        }catch (Exception e){
            e.printStackTrace();
        }
    }
    public void weighted_perlin_test(){
        Perlin perlin = new Perlin();
        perlin.setUp(w,h,40,this);
        float[][] noise = perlin.perlinPattern();
        float[][] lattice = new float[h][w];
        for(int i=0;i<h;i++){
            for(int j=0;j<h;j++){
                if(i>h/2){
                    lattice[i][j] = 0.9f * 255;
                }else{
                    lattice[i][j] = 0.9f * 255;
                }
            }
        }
        Vec4[][] colors = new Vec4[h][w];
        for(int i=0;i<h;i++){
            for(int j=0;j<h;j++){
                float t = Math.abs(noise[i][j] *= lattice[i][j]);
                //System.out.println(noise[i][j]);
                if(t > 255){
                    t = 255;
                }
                colors[i][j] = new Vec4(t,t,t,255);
            }
        }
        try{
            ImageHelper.saveAsPng(colors,w,h,"src/main/resources/weighted_perlin.png");
        }catch (Exception e){
            e.printStackTrace();
        }
        System.out.println(total);
    }
    public static void main(String[] args){
        new NoiseTest().dpd_test();
    }
}
