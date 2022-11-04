package com.rust.util;

import com.rust.model.PipelineConfig;
import com.rust.model.PipelineManager;
import com.rust.model.PipelineState;
import com.rust.view.IAction;
import com.rust.view.window.ControllTab;
import com.rust.viewModel.ControllableArgument;
import glm.vec._4.Vec4;
import org.lwjgl.nuklear.NkColorf;

public class StartPipelineHandler implements IAction {
    @Override
    public void act() {
        //if(PipelineManager.shared.state.equals(PipelineState.OPEN)){
            PipelineConfig config = new PipelineConfig();
            config.generateImage = false;
            config.cc = ControllTab.instance.windowArgDictionary.get("CC").buffer.get(0);
            config.rh = ControllTab.instance.windowArgDictionary.get("RH").buffer.get(0);
            config.sc = ControllTab.instance.windowArgDictionary.get("SC").buffer.get(0);
            config.oc = ControllTab.instance.windowArgDictionary.get("OC").buffer.get(0);
            config.temperature = ControllTab.instance.windowArgDictionary.get("temperature").buffer.get(0);
            config.textureWidth = config.textureHeight = 200;
            config.backTexture = new Vec4[200][200];
            config.curvature = CurvatureHelper.generate(ControllTab.instance.commonCurvature.path);
            config.metallic = new float[200][200];
            config.roughness = new float[200][200];
            config.growTime = ControllTab.instance.windowArgDictionary.get("grow_time").buffer.get(0);
            float meta = ControllTab.instance.windowArgDictionary.get("metallic").buffer.get(0);
            float rough = ControllTab.instance.windowArgDictionary.get("roughness").buffer.get(0);
            for(int i=0;i<200;i++){
                for(int j=0;j<200;j++){
                    NkColorf colorf = ControllTab.instance.background;
                    config.backTexture[i][j] = new Vec4(colorf.r()*255,colorf.g()*255,colorf.b()*255,colorf.a()*255);
                    config.metallic[i][j] = meta;
                    config.roughness[i][j] = rough;
                }
            }
            PipelineManager.shared.startPipeline(config);
        }
    //}
}
