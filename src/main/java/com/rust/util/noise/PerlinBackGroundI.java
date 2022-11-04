package com.rust.util.noise;

import glm.vec._2.Vec2;

/**
 * 将基于此接口扭曲perlin
 */
public interface PerlinBackGroundI {
    default float twist(Vec2 pos, Perlin perlin){
        return 0;
    }
    default float fade(float t){
        return t* t* t* (t * (6 * t - 15) + 10);
    }
}
