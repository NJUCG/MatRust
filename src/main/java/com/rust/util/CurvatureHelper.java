package com.rust.util;

import com.jogamp.opengl.util.GLBuffers;
import com.sun.jna.platform.win32.WinBase;
import glm.vec._4.Vec4;
import org.lwjgl.stb.STBImage;

import javax.imageio.ImageIO;
import javax.imageio.ImageReader;
import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;
import java.io.File;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

public class CurvatureHelper {
    public static float sigma = 1f;
    private static float _2squreSigmaDe = 0f;
    private static float _2squreSigmaPIDe = 0f;
    // 通过法线图生成曲率图
    public static float[][] generate(String path,int blurRadius){
        File file = new File(path);
        _2squreSigmaDe = 1/(2*sigma*sigma);
        _2squreSigmaPIDe = (float) (_2squreSigmaDe / Math.PI);
        BufferedImage img = null;
        try {
            img = ImageIO.read(file);
        }catch (Exception e){
            e.printStackTrace();
        }
        assert img != null;
        int w = img.getWidth();
        int h = img.getHeight();
        float[][] res = new float[h][w];
        int nrComponents = img.getColorModel().getNumComponents();
        if(nrComponents == 3){
            for(int i=0;i<h;i++){
                for(int j=0;j<w;j++){
                    int rgb = img.getRGB(j,i);
                    int b = rgb & 0xFF;
                    res[i][j] = Math.abs(b);
                }
            }
        }else{
            for(int i=0;i<h;i++){
                for(int j=0;j<w;j++){
                    int rgb = img.getRGB(j,i);
                    int b = (rgb & 0xFF00)>>8;
                    res[i][j] = Math.abs(b);
                }
            }
        }
        //
        float[][] horizontal_diff = new float[h][w];
        for(int i=0;i<h;i++){
            for(int j=0;j<w-1;j++){
                horizontal_diff[i][j] = res[i][j+1] -res[i][j];
            }
        }
        float[][] vertical_diff = new float[h][w];
        for(int i=0;i<h-1;i++){
            for(int j=0;j<w;j++){
                vertical_diff[i][j] = res[i+1][j] -res[i][j];
            }
        }
        for(int i=0;i<h;i++){
            for(int j=0;j<w-1;j++){
                res[i][j] = (horizontal_diff[i][j] + vertical_diff[i][j])/2;
            }
        }
        //模糊
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                float t = 0;
                for(int di=-blurRadius;di<=blurRadius;di++){
                    for(int dj=-blurRadius;dj<=blurRadius;dj++){
                        int ni = i + di;
                        int nj = j + dj;
                        if(ni>=0&&ni<h&&nj>=0&&nj<w){
                            t += (res[ni][nj] * gs(dj,di));
                        }
                    }
                }
                horizontal_diff[i][j] = Math.min(t,255);
            }
        }
        for(int i=0;i<h;i++){
            System.arraycopy(horizontal_diff[i],0,res[i],0,w);
        }
        /*Vec4[][] temp = new Vec4[h][w];
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                float t = res[i][j];
                temp[i][j] = new Vec4(t,t,t,255);
            }
        }
        try {
            ImageHelper.saveAsPng(temp,w,h,"src/main/resources/curve.png");
        }catch (Exception e){
            e.printStackTrace();
        }*/
        return res;
    }

    // 直接获取曲率图
    public static float[][] generate(String path){
        File file = new File(path);
        BufferedImage img = null;
        try {
            img = ImageIO.read(file);
        }catch (Exception e){
            e.printStackTrace();
        }
        assert img != null;
        int w = img.getWidth();
        int h = img.getHeight();
        float[][] res = new float[h][w];
        int nrComponents = img.getColorModel().getNumComponents();
        if(nrComponents == 3){
            for(int i=0;i<h;i++){
                for(int j=0;j<w;j++){
                    int rgb = img.getRGB(j,i);
                    int b = rgb & 0xFF;
                    res[i][j] = 255 - Math.abs(b);
                }
            }
        }else{
            for(int i=0;i<h;i++){
                for(int j=0;j<w;j++){
                    int rgb = img.getRGB(j,i);
                    int b = (rgb & 0xFF00)>>8;
                    res[i][j] = 255 - Math.abs(b);
                }
            }
        }
        return res;
    }
    // 获取曲率图并将其压缩至指定长宽
    public static float[][] generate(String path,int w,int h){
        File file = new File(path);
        BufferedImage img = null;
        try {
            img = ImageIO.read(file);
        }catch (Exception e){
            e.printStackTrace();
        }
        assert img != null;
        int ow = img.getWidth();
        int oh = img.getHeight();
        float[][] origin = new float[oh][ow];
        float[][] res = new float[h][w];
        int nrComponents = img.getColorModel().getNumComponents();
        if(nrComponents == 3){
            for(int i=0;i<oh;i++){
                for(int j=0;j<ow;j++){
                    int rgb = img.getRGB(j,i);
                    int b = rgb & 0xFF;
                    origin[i][j] = 255 - Math.abs(b);
                }
            }
        }else{
            for(int i=0;i<oh;i++){
                for(int j=0;j<ow;j++){
                    int rgb = img.getRGB(j,i);
                    int b = (rgb & 0xFF00)>>8;
                    origin[i][j] = 255 - Math.abs(b);
                }
            }
        }
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){

            }
        }
        return res;
    }
    private static float gs(int dx,int dy){
        float exp = -(dx*dx+dy*dy)*_2squreSigmaDe;
        return (float) (_2squreSigmaPIDe * Math.exp(exp));
    }
}
