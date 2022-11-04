package com.rust.util;

import com.jogamp.opengl.util.GLBuffers;
import glm.vec._2.Vec2;
import glm.vec._3.Vec3;

import java.nio.FloatBuffer;
import java.util.List;

public class Vertex {
    public static int BYTES(){
        return 14 * Float.BYTES;
    }
    public static int offsetBy(VertexAttrib attrib){
        switch (attrib){
            case Position:
                return 0;
            case Normal:
                return 3 * Float.BYTES;
            case TexCoords:
                return 6 * Float.BYTES;
            case Tangent:
                return 8 * Float.BYTES;
            case BitTangent:
                return 11 * Float.BYTES;
        }
        return -1;
    }
    public static FloatBuffer toFb(List<Vertex> vertices){
        float[] fa = new float[BYTES()*vertices.size()];
        int index=0;
        for (Vertex v: vertices) {
            fa[index++] = v.Position.x;
            fa[index++] = v.Position.y;
            fa[index++] = v.Position.z;
            fa[index++] = v.Normal.x;
            fa[index++] = v.Normal.y;
            fa[index++] = v.Normal.z;
            fa[index++] = v.TexCoords.x;
            fa[index++] = v.TexCoords.y;
            fa[index++] = v.Tangent.x;
            fa[index++] = v.Tangent.y;
            fa[index++] = v.Tangent.z;
            fa[index++] = v.BitTagnent.x;
            fa[index++] = v.BitTagnent.y;
            fa[index++] = v.BitTagnent.z;
        }
        return GLBuffers.newDirectFloatBuffer(fa);
    }

    public Vec3 Position;
    public Vec3 Normal;
    public Vec2 TexCoords;
    public Vec3 Tangent;
    public Vec3 BitTagnent;
}
