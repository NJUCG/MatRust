package com.rust.view.window;

import com.rust.util.UIUtil;
import com.rust.view.window.style.Style;
import com.rust.viewModel.ControllableArgument;
import org.lwjgl.nuklear.*;
import org.lwjgl.system.MemoryStack;

import java.awt.*;

import static org.lwjgl.nuklear.Nuklear.*;
import static org.lwjgl.nuklear.Nuklear.nk_end;

public class ControllTab extends MyWindow{
    // 环境参数
    // 引擎参数
    public static ControllTab instance = new ControllTab();
    private ControllTab(){
        newByteBuffer("env_tree");
        newByteBuffer("engine_tree");
        newPointerBuffer("0",2);

        newByteBuffer("test_tree");
        newWindowArg("test");

        newByteBuffer("RH_tree");
        newWindowArg("RH");

        newByteBuffer("CC_tree");
        newWindowArg("CC");

        newByteBuffer("SC_tree");
        newWindowArg("SC");

        newByteBuffer("OC_tree");
        newWindowArg("OC");

        newByteBuffer("temperature_tree");
        newWindowArg("temperature");

        newByteBuffer("curvature_tree");
        newByteBuffer("background_color_tree");

        newByteBuffer("metallic_tree");
        newWindowArg("metallic");

        newByteBuffer("roughness_tree");
        newWindowArg("roughness");

        newByteBuffer("grow_time_tree");
        newWindowArg("grow_time");

        background.set(1,0.313f,0.235f,1);
    }
    public WindowImage commonCurvature = new WindowImage();
    public WindowImage textureCurvature = new WindowImage();
    public NkColorf background = NkColorf.create();
    public void layout(NkContext ctx,int x,int y,int rectH,int rectW) {
        seed = -1;
        try (MemoryStack stack = MemoryStack.stackPush()) {
            ctx = Style.style.controllTabStyle(ctx);
            NkRect rect = NkRect.malloc(stack);
            if (nk_begin(
                    ctx,
                    "arguments",
                    nk_rect(x, y, rectW, rectH, rect),
                    NK_WINDOW_MINIMIZABLE
            )) {
                if(nk_tree_push_hashed(ctx,NK_TREE_TAB,"Env",NK_MINIMIZED,getBBuffer("env_tree"),nextSeed())){
                    addPercentArg(ctx,"RH",0,1,0.01f);
                    addPercentArg(ctx,"CC",0,1,0.01f);
                    addPercentArg(ctx,"SC",0,1,0.01f);
                    addPercentArg(ctx,"OC",0,1,0.01f);
                    addPercentArg(ctx,"temperature",273,373,1f);
                    addPercentArg(ctx,"roughness",0,1,0.01f);
                    addPercentArg(ctx,"metallic",0,1,0.01f);
                    nk_tree_pop(ctx);
                }
                if(nk_tree_push_hashed(ctx,NK_TREE_TAB,"Engine",NK_MINIMIZED,getBBuffer("engine_tree"),nextSeed())){
                    addPercentArg(ctx,"Grow Time","grow_time",0,200,1);
                    if(nk_tree_push_hashed(ctx,NK_TREE_NODE,"Background Color",NK_MINIMIZED,getBBuffer("background_color_tree"),nextSeed())){
                        if (nk_combo_begin_color(ctx, NkColor.create().set((byte) (255 *background.r()), (byte) (255 *background.g()),(byte) (255 *background.b()),(byte) (255 *background.a())), nk_vec2(nk_widget_width(ctx),400,NkVec2.create()))) {
                            nk_layout_row_dynamic(ctx, 120, 1);
                            background = nk_color_picker(ctx, background, NK_RGBA);
                            nk_layout_row_dynamic(ctx, 25, 1);
                            background.r(nk_propertyf(ctx,"#R:",0,background.r(),1,0.01f,0.01f));
                            background.g(nk_propertyf(ctx,"#G:",0,background.g(),1,0.01f,0.01f));
                            background.b(nk_propertyf(ctx,"#B:",0,background.b(),1,0.01f,0.01f));
                            background.a(nk_propertyf(ctx,"#A:",0,background.a(),1,0.01f,0.01f));
                            nk_combo_end(ctx);
                        }
                        nk_tree_pop(ctx);
                    }
                    if(nk_tree_push_hashed(ctx,NK_TREE_TAB,"Common Curvature",NK_MINIMIZED,getBBuffer("curvature_tree"),nextSeed())){
                        FilePanel.layoutTexturePicker(ctx,commonCurvature);
                        nk_tree_pop(ctx);
                    }
                    nk_tree_pop(ctx);
                }
                nk_end(ctx);
            }
        }
    }
    void addPercentArg(NkContext ctx,String name,float min,float max,float step){
        if(nk_tree_push_hashed(ctx,NK_TREE_NODE,name,NK_MINIMIZED,getBBuffer(name+"_tree"),nextSeed())){
            nk_layout_row(ctx,NK_DYNAMIC,40,new float[]{0.6f,0.25f,0.2f});
            nk_slider_float(ctx,min,getWindowArg(name).buffer,max,step);
            nk_property_float(ctx,"",min,getWindowArg(name).buffer,max,step,step);
            nk_tree_pop(ctx);
        }
    }
    void addPercentArg(NkContext ctx,String title,String name,float min,float max,float step){
        if(nk_tree_push_hashed(ctx,NK_TREE_NODE,title,NK_MINIMIZED,getBBuffer(name+"_tree"),nextSeed())){
            nk_layout_row(ctx,NK_DYNAMIC,40,new float[]{0.6f,0.25f,0.2f});
            nk_slider_float(ctx,min,getWindowArg(name).buffer,max,step);
            nk_property_float(ctx,"",min,getWindowArg(name).buffer,max,step,step);
            nk_tree_pop(ctx);
        }
    }

}
