package com.rust.viewModel;

import com.jogamp.opengl.util.GLBuffers;
import com.rust.model.PipelineConfig;
import com.rust.model.PipelineManager;
import com.rust.model.PipelineState;
import com.rust.util.CurvatureHelper;
import com.rust.viewModel.argumentManager.*;
import glm.vec._4.Vec4;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.AdjustmentListener;
import java.nio.IntBuffer;
import java.util.Dictionary;
import java.util.Hashtable;

import static org.lwjgl.opengl.GL11C.glGenTextures;

public class ControllPanelViewModel implements ActionListener {
    public static ControllPanelViewModel shared = new ControllPanelViewModel();
    public ControllPanelViewModelDelegate delegate;
    public volatile boolean shouldStartPipeline = false;
    private ControllPanelViewModel(){
    }
    public void frameFun(){
        if (shouldStartPipeline){
            shouldStartPipeline = false;
            PipelineConfig config = new PipelineConfig();
            config.generateImage = false;
            config.test = argumentManagers.get(ControllableArgument.TEST).value();
            config.cc = argumentManagers.get(ControllableArgument.CC).value();
            config.rh = argumentManagers.get(ControllableArgument.RH).value();
            config.sc = argumentManagers.get(ControllableArgument.SC).value();
            config.oc = argumentManagers.get(ControllableArgument.OC).value();
            config.temperature = argumentManagers.get(ControllableArgument.Temperature).value();
            config.textureWidth = config.textureHeight = 200;
            config.backTexture = new Vec4[200][200];
            config.curvature = CurvatureHelper.generate("src/main/resources/models/stranger/curve.png");
            config.metallic = new float[200][200];
            config.roughness = new float[200][200];
            for(int i=0;i<200;i++){
                for(int j=0;j<200;j++){
                    config.backTexture[i][j] = new Vec4(255,80,60,255);
                    config.metallic[i][j] = 1.0f;
                    config.roughness[i][j] = 0.1f;
                }
            }
            PipelineManager.shared.startPipeline(config);
        }
    }
    public void setUp(ControllPanelViewModelDelegate delegate){
        this.delegate=delegate;
        argumentManagers.put(ControllableArgument.TEST,new TestControllableArgumentManager());
        argumentManagers.put(ControllableArgument.RH,new RHControllableArgumentManager());
        argumentManagers.put(ControllableArgument.OC,new OCControllableArgumentManager());
        argumentManagers.put(ControllableArgument.SC,new SCControllableArgumentManager());
        argumentManagers.put(ControllableArgument.CC,new CCControllableArgumentManager());
        argumentManagers.put(ControllableArgument.Temperature,new TemperatureArgumentManager());
        delegate.addArgument(ControllableArgument.TEST);
        delegate.addArgument(ControllableArgument.RH);
        delegate.addArgument(ControllableArgument.CC);
        delegate.addArgument(ControllableArgument.SC);
        delegate.addArgument(ControllableArgument.OC);
        delegate.addArgument(ControllableArgument.Temperature);
    }

    private final Dictionary<ControllableArgument,ControllableArgumentManager> argumentManagers = new Hashtable<>();

    public String getArgumentName(ControllableArgument arg){
        return argumentManagers.get(arg).getName();
    }
    public AdjustmentListener getAdjustListener(ControllableArgument argument){
        return argumentManagers.get(argument);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String label = ((Button)e.getSource()).getLabel();
        if(label.equals("Start")){
            if(PipelineManager.shared.state!=PipelineState.OPEN){
                return;
            }
            shouldStartPipeline = true;
        }else{
            if(PipelineManager.shared.state!=PipelineState.OPEN){
                return;
            }
            PipelineManager.shared.clear();
        }
    }

}
