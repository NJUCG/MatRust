import com.rust.model.PipelineConfig;
import com.rust.model.PipelineManager;

public class PipelineTest {
    PipelineManager manager = PipelineManager.shared;
    int w = 200;
    int h = 200;
    public void oxideLayer(){
        PipelineConfig config = new PipelineConfig();
        config.oc = 0.2;
        config.textureWidth = config.textureHeight = w;
        config.temperature = 40;
        manager.startPipeline(config);

    }
    public static void main(String[] args) {
        System.out.println(System.getProperty("java.library.path"));
    }
}
