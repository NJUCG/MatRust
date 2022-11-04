package com.rust.model.layers;

import com.rust.model.PipelineConfig;
import glm.vec._4.Vec4;

public class BaseLayer implements LayerManager{
    public RustUnit[][] units;
    protected int w,h;
    @Override
    public void acceptRules(PipelineConfig config) {
        w = config.textureWidth;
        h = config.textureHeight;
        units = new RustUnit[config.textureHeight][config.textureWidth];
    }

    @Override
    public void rust(double deltaTime) {

    }

    @Override
    public float[][] getHeight() {
        return new float[0][];
    }

    @Override
    public RustUnit[][] getUnits() {
        return units;
    }
}
