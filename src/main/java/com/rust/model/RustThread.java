package com.rust.model;

import com.rust.util.TimeManager;

public class RustThread extends Thread{
    private double currentTime = 0;
    private double totalTime = 0;

    private PipelineManager manager;
    public void refresh(){
        manager = PipelineManager.shared;
        PipelineConfig pipelineConfig = PipelineManager.shared.config;
        totalTime = pipelineConfig.growTime;
    }
    @Override
    public void run() {
        while (currentTime<totalTime) {
            double deltaTime = TimeManager.shared.deltaTime;
            currentTime+=deltaTime;
            manager.generateRust(deltaTime);
            try{
                manager.shouldMerge = true;
            }catch (Exception e){
                e.printStackTrace();
            }
        }
        currentTime = totalTime = 0;
        manager.completePipeline();
    }
}
