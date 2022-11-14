package com.rust.util;

import com.rust.view.window.WindowImage;
import org.lwjgl.nuklear.NkContext;
import org.lwjgl.nuklear.NkImage;

import static org.lwjgl.nuklear.Nuklear.*;

public class UIUtil {
    public static void ui_header(NkContext ctx, String title){
        nk_layout_row_dynamic(ctx,20,1);
        nk_label(ctx,title,NK_TEXT_ALIGN_LEFT);
    }
    public static void ui_widget_centered(NkContext ctx, float height)
    {
        float ratio[] = {0.15f, 0.50f, 0.35f};
        nk_layout_row(ctx, NK_DYNAMIC, height, ratio);
        nk_spacing(ctx, 1);
    }
    public static void ui_header(NkContext ctx, String title,Float data){
        float ratio[] = {0.30f, 0.70f};
        nk_layout_row(ctx, NK_DYNAMIC, 20, ratio);
        nk_label(ctx,title,NK_TEXT_ALIGN_LEFT);
        nk_label(ctx,data.toString(),NK_TEXT_ALIGN_LEFT);
    }
    public static void ui_widget(NkContext ctx, float height)
    {
        float ratio[] = {0.10f, 0.90f};
        nk_layout_row(ctx, NK_DYNAMIC, height, ratio);
        nk_spacing(ctx, 1);
    }
    public static void ui_widget(NkContext ctx, float height,float[] ratio)
    {
        nk_layout_row(ctx, NK_DYNAMIC, height, ratio);
        nk_spacing(ctx, 1);
    }
    public static void ui_static_widget(NkContext ctx,float height,float[] width){
        nk_layout_row(ctx,NK_STATIC,height,width);
        nk_spacing(ctx,1);
    }
    public static NkImage create_image(String name){
        NkImage image = NkImage.create();
        int id = -1;
        try{
            id = ImageHelper.fromUI(name);
        }catch (Exception e){
            e.printStackTrace();
        }
        int finalId = id;
        image.handle(it->it.id(finalId));
        return image;
    }
    public static NkImage create_image(String name,String dir){
        NkImage image = NkImage.create();
        int id = -1;
        try{
            id = ImageHelper.from(name,dir);
        }catch (Exception e){
            e.printStackTrace();
        }
        int finalId = id;
        image.handle(it->it.id(finalId));
        return image;
    }
    public static WindowImage create_wimage(String name){
        WindowImage img = new WindowImage();
        img.name = name;
        img.image = NkImage.create();
        int id = -1;
        try{
            id = ImageHelper.fromUI(img.name);
        }catch (Exception e){
            e.printStackTrace();
        }
        int finalId = id;
        img.textureId = finalId;
        img.image.handle(it->it.id(finalId));
        return img;
    }
    public static WindowImage create_wimage(String name,String dir){
        WindowImage img = new WindowImage();
        img.name = name;
        img.path = dir + "\\" + name;
        img.image = NkImage.create();
        int id = -1;
        try{
            id = ImageHelper.from(img.name,dir);
        }catch (Exception e){
            e.printStackTrace();
        }
        int finalId = id;
        img.textureId = finalId;
        img.image.handle(it->it.id(finalId));
        return img;
    }
}
