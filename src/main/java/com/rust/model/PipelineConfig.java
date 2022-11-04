package com.rust.model;

import glm.vec._4.Vec4;

public class PipelineConfig {
    public double oc;
    public double cc;
    public double sc;
    public double rh;
    public double test;
    public double temperature;
    public double growTime = 40;
    public int textureWidth = 400;
    public int textureHeight = 400;
    public boolean generateImage = false;
    public String diffPath = "diff.png";
    public Vec4[][] backTexture;
    public float[][] metallic;
    public float[][] roughness;
    public boolean useCurvature = true;
    public float[][] curvature = null;
}
