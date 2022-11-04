package com.rust.model.layers;

import com.rust.model.PipelineConfig;

/**
 * 该接口用于控制每一层的生成
 */
public interface LayerManager {
    void acceptRules(PipelineConfig config);
    void rust(double deltaTime);
    float[][] getHeight();
    RustUnit[][] getUnits();
}
