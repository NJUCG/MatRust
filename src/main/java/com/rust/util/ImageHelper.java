package com.rust.util;

import glm.vec._4.Vec4;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class ImageHelper {
    public static void saveAsPng(Vec4[][] data,int width,int height,String path) throws IOException {
        BufferedImage image = new BufferedImage(width,height,BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = image.createGraphics();
        g.setColor(Color.black);
        g.fillRect(0,0,width,height);
        for (int i=0;i<width;i++){
            for(int j=0;j<height;j++){
                Vec4 v = data[j][i];
                try{
                    g.setColor(new Color(Math.min((int)(v.x),255),Math.min((int)(v.y),255),Math.min((int)(v.z),255),Math.min((int)(v.w),255)));
                    g.fillRect(i,j,1,1);
                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        }
        File f = new File(path);
        ImageIO.write(image,"PNG",f);
    }
    public static int fromTexture(String name) throws Exception {
        return Model.TextureFromFile(name,"src/main/resources/textures").get(0);
    }
    public static int fromUI(String name) throws Exception {
        return Model.TextureFromFile(name,"src/main/resources/ui").get(0);
    }
    public static int from(String name,String dir) throws Exception{
        return Model.TextureFromFile(name,dir).get(0);
    }
}
