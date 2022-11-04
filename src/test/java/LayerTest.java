import com.rust.model.PipelineConfig;
import com.rust.model.layers.OxideLayer;
import com.rust.model.layers.RustUnit;
import com.rust.util.GlmExtension;
import com.rust.util.ImageHelper;
import glm.vec._4.Vec4;

public class LayerTest {
    public void oxideLayer(){
        float time = 20;
        int w,h;
        w=h=200;
        OxideLayer layer = new OxideLayer();
        PipelineConfig config = new PipelineConfig();
        config.oc = 0.2;
        config.textureWidth = config.textureHeight = w;
        config.temperature = 40;
        layer.acceptRules(config);
        layer.rust(time);
        RustUnit[][] units = layer.getUnits();
        Vec4[][] colors = new Vec4[h][w];
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                colors[i][j] = GlmExtension.mul(units[i][j].color,units[i][j].thickness);
                if (colors[i][j].x > 255){
                    colors[i][j].x = 255;
                }
                if (colors[i][j].y > 255){
                    colors[i][j].y = 255;
                }
                if (colors[i][j].z > 255){
                    colors[i][j].z = 255;
                }
                colors[i][j].w = 255;
            }
        }
        try {
            ImageHelper.saveAsPng(colors,w,h,"src/main/resources/oxideLayer.png");
        }catch (Exception e){
            e.printStackTrace();
        }

    }
    public static void main(String[] args){
        String s = "sdf";
        String obj = s;
        System.out.println(obj);
        s= "1213";
        System.out.println((String)obj);

    }
}
