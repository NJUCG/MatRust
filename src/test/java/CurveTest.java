import com.rust.util.CurvatureHelper;
import com.rust.view.window.LayerInfo;

import java.util.ArrayList;
import java.util.List;

public class CurveTest {
    void curveTest(){
        CurvatureHelper helper = new CurvatureHelper();
        CurvatureHelper.generate("src/main/resources/models/stranger/curve.png");
    }

    public static void main(String[] args) {
        List<LayerInfo> test = new ArrayList<>();
        test.add(new LayerInfo("test0"));
        LayerInfo info =new LayerInfo("test1");
    }
}
