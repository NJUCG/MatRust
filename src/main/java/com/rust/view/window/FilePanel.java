package com.rust.view.window;

import com.rust.util.Texture;
import com.rust.util.UIUtil;
import com.rust.view.window.style.Style;
import org.lwjgl.nuklear.*;
import org.lwjgl.system.MemoryStack;

import java.awt.*;
import java.util.Dictionary;
import java.util.Enumeration;
import java.util.Hashtable;

import static org.lwjgl.nuklear.Nuklear.*;

public class FilePanel extends MyWindow {
    public static FilePanel instance = new FilePanel();
    public Dictionary<Integer,WindowImage> imageDictionary = new Hashtable<>();
    public static void layoutTexturePicker(NkContext ctx,WindowImage img){
        nk_layout_row_dynamic(ctx,20,1);
        if(nk_combo_begin_label(ctx, img.textureId + "/" + img.name,nk_vec2(200,200, NkVec2.create()))){
            nk_layout_row(ctx,NK_DYNAMIC,25,new float[]{1});
            Enumeration<Integer> t = instance.imageDictionary.keys();
            while(t.hasMoreElements()){
                Integer i = t.nextElement();
                Integer textureId = instance.imageDictionary.get(i).textureId;
                String name = instance.imageDictionary.get(i).name;
                String path = instance.imageDictionary.get(i).path;
                if (nk_combo_item_label(ctx, textureId.toString() + "/" + name, NK_TEXT_LEFT)) {
                    img.textureId = textureId;
                    img.name = name;
                    img.path = path;
                }
            }
            nk_combo_end(ctx);
        }
    }
    private FilePanel(){
        newByteBuffer("0");
        newByteBuffer("1");
    }
    public void initAfterOpenGL(){
        WindowImage temp;
        temp = UIUtil.create_wimage("normal.png");
        imageDictionary.put(temp.textureId,temp);
    }
    public void addImage(String dir,String name){
        WindowImage temp;
        temp = UIUtil.create_wimage(name,dir);
        imageDictionary.put(temp.textureId,temp);

    }
    String browse_mode = "icons";
    public void layout(NkContext ctx,int x, int y, int rectH, int rectW){
        seed = -1;
        try (MemoryStack stack = MemoryStack.stackPush()) {
            ctx = Style.style.controllTabStyle(ctx);
            NkRect rect = NkRect.malloc(stack);
            if (nk_begin(
                    ctx,
                    "file",
                    nk_rect(x, y, rectW, rectH, rect),
                     NK_WINDOW_MINIMIZABLE
            )) {
                if(nk_tree_push_hashed(ctx,NK_TREE_NODE,"textures",NK_MINIMIZED,getBBuffer("0"),nextSeed())){
                    float width = 100;
                    nk_layout_row(ctx,NK_STATIC,30,new float[]{50,width,20,width});
                    nk_label(ctx,"align",NK_TEXT_LEFT);
                    if(nk_combo_begin_label(ctx,browse_mode, NkVec2.create().set(width,width))){
                        String temp = "icons";
                        if(browse_mode.equals(temp)){
                            temp = "list";
                        }
                        nk_layout_row(ctx,NK_DYNAMIC,30,new float[]{1});
                        if(nk_combo_item_label(ctx,temp,NK_TEXT_LEFT)){
                            browse_mode = temp;
                        }
                        nk_combo_end(ctx);
                    }
                    nk_spacing(ctx,1);
                    chooseTextureLabel(ctx);
                    if(browse_mode.equals("icons")){
                        int imgWidth = 100;
                        int groupHeight = 170;
                        int spacing = 10;
                        nk_layout_row(ctx,NK_DYNAMIC,500,new float[]{1});
                        Enumeration<Integer> t = imageDictionary.keys();
                        if(nk_group_begin(ctx,"tex",NK_WINDOW_BORDER)) {
                            nk_layout_row(ctx, NK_STATIC, groupHeight, new float[]{imgWidth, spacing, imgWidth, spacing, imgWidth, spacing});
                            while (t.hasMoreElements()) {
                                WindowImage img = imageDictionary.get(t.nextElement());
                                if (nk_group_begin(ctx, img.name, NK_WINDOW_NO_SCROLLBAR)) {
                                    nk_layout_row(ctx, NK_STATIC, 80, new float[]{2.5f, 80});
                                    nk_spacing(ctx, 1);
                                    nk_image(ctx, img.image);
                                    nk_layout_row(ctx, NK_STATIC, 20, new float[]{2.5f, 80});
                                    nk_spacing(ctx, 1);
                                    nk_label(ctx, img.name, NK_TEXT_CENTERED);
                                    nk_spacing(ctx, 1);
                                    nk_label(ctx, "id:" + img.textureId, NK_TEXT_CENTERED);
                                    nk_spacing(ctx, 1);
                                    if (nk_button_label(ctx, "delete")) {
                                        imageDictionary.remove(img.textureId);
                                    }
                                    nk_group_end(ctx);
                                }
                                nk_spacing(ctx,1);
                            }
                            nk_group_end(ctx);
                        }
                    }
                    else{
                        Enumeration<Integer> t = imageDictionary.keys();
                        while (t.hasMoreElements()) {
                            WindowImage img = imageDictionary.get(t.nextElement());
                            nk_layout_row(ctx,NK_DYNAMIC,100,new float[]{1});
                            if (nk_group_begin(ctx, img.name, NK_WINDOW_NO_SCROLLBAR)) {
                                nk_layout_row(ctx,NK_STATIC,80,new float[]{20,10,80,10,80,10,80});
                                nk_label(ctx,Integer.toString(img.textureId),NK_TEXT_CENTERED);
                                nk_spacing(ctx,1);
                                nk_label(ctx,img.name,NK_TEXT_CENTERED);
                                nk_spacing(ctx,1);
                                nk_image(ctx,img.image);
                                nk_spacing(ctx,1);
                                if(nk_button_label(ctx,"delete")){
                                    imageDictionary.remove(img.textureId);
                                }
                                nk_group_end(ctx);
                            }
                        }
                    }
                    nk_tree_pop(ctx);
                }
                if(nk_tree_push_hashed(ctx,NK_TREE_NODE,"model",NK_MINIMIZED,getBBuffer("1"),nextSeed())){
                    chooseModelLabel(ctx);
                    nk_tree_pop(ctx);
                }
                nk_end(ctx);
            }
        }
    }
    public void chooseTextureLabel(NkContext ctx){
        if(nk_button_label(ctx,"new")){
            Frame frame = new Frame("hh");
            FileDialog fileDialog = new FileDialog(frame,"choose",FileDialog.LOAD);
            frame.setVisible(false);
            fileDialog.setVisible(true);
            String file = fileDialog.getFile();
            if(file!=null){
                addImage(fileDialog.getDirectory(),file);
            }
        }
    }
    public String model_path = "";
    public String model_dir = "";
    public void chooseModelLabel(NkContext ctx){
        if(nk_button_label(ctx,"new")){
            Frame frame = new Frame("hh");
            FileDialog fileDialog = new FileDialog(frame,"choose",FileDialog.LOAD);
            frame.setVisible(false);
            fileDialog.setVisible(true);
            String file = fileDialog.getFile();
            if(file!=null){
                model_path = fileDialog.getDirectory() + "\\" + file;
                model_dir = fileDialog.getDirectory();
            }
        }
    }
}
