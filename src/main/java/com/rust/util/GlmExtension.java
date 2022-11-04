package com.rust.util;

import glm.mat._4.Mat4;
import glm.vec._2.Vec2;
import glm.vec._3.Vec3;
import glm.vec._4.Vec4;
import jglm.Vec;

import javax.swing.plaf.PanelUI;

public class GlmExtension {
    public static Vec3 add(Vec3 a,Vec3 b){
        return new Vec3(a.x+b.x,a.y+b.y,a.z+b.z);
    }
    public static Vec4 add(Vec4 a,Vec4 b){
        return new Vec4(a.x+b.x,a.y+b.y,a.z+b.z,a.w+b.w);
    }
    public static Vec3 sub(Vec3 a,Vec3 b){
        return new Vec3(a.x-b.x,a.y-b.y,a.z-b.z);
    }
    public static Vec3 mul(Vec3 a,float b){
        return new Vec3(a.x*b,a.y*b,a.z*b);
    }
    public static Vec2 mul(Vec2 a,float b){
        return new Vec2(a.x*b,a.y*b);
    }
    public static Vec4 mul(Vec4 a, float b){
        return new Vec4(a.x*b,a.y*b,a.z*b,a.w*b);
    }
    public static Vec3 normalize(Vec3 a) {
        float fX = a.x;
        float fY = a.y;
        float fZ = a.z;
        float inverseSqrt = 1.0F / (float)Math.sqrt((double)(fX * fX + fY * fY + fZ * fZ));
        fX *= inverseSqrt;
        fY *= inverseSqrt;
        fZ *= inverseSqrt;
        return new Vec3(fX,fY,fZ);
    }

    public static Vec3 cross(Vec3 x,Vec3 y){
        return new Vec3(
                x.y * y.z - y.y * x.z,
                x.z * y.x - y.z * x.x,
                x.x * y.y - y.x * x.y);
    }
    public static float dot(Vec3 x,Vec3 y) {
        return x.x * y.x + x.y * y.y + x.z * y.z;
    }
    public static float dot(Vec2 x, Vec2 y) {
        return x.x * y.x + x.y * y.y;
    }
    public static Mat4 lookAtRH(Vec3 eye,Vec3 center,Vec3 up){
        Vec3 f = normalize(sub(center,eye));
        Vec3 s = normalize(cross(f, up));
        Vec3 u = cross(s, f);
        Mat4 res = new Mat4(1.0f);
        res.m00 = s.x;
        res.m01 = u.x;
        res.m02 = -f.x;
        res.m03 = 0.0F;
        res.m10 = s.y;
        res.m11 = u.y;
        res.m12 = -f.y;
        res.m13 = 0.0F;
        res.m20 = s.z;
        res.m21 = u.z;
        res.m22 = -f.z;
        res.m23 = 0.0F;
        res.m30 = -dot(s,eye);
        res.m31 = -dot(u,eye);
        res.m32 = dot(f,eye);
        res.m33 = 1.0F;
        return res;
    }

    public static float radians(float zoom) {
        return zoom * 0.01745329251994329576923690768489f;
    }
}
