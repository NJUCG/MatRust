package com.rust.view.window;

import com.jogamp.opengl.util.GLBuffers;
import com.rust.model.FilmComposition;
import com.rust.util.ColorHelper;
import com.rust.util.Texture;
import glm.vec._4.Vec4;
import org.lwjgl.nuklear.*;
import org.lwjgl.system.MemoryStack;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import static org.lwjgl.nuklear.Nuklear.*;

public class LayerNode extends Node{
    public LayerNode(){
        title = "Layer";
        x = y = 50;
        w = 200;
        h = 400;
        NodeSlot nodeInfoSlot = new NodeSlot();
        nodeInfoSlot.dataType = LayerInfo.class;
        nodeInfoSlot.data = info;
        outputSlots.add(nodeInfoSlot);

        LayerInfo oxideLayer = new LayerInfo("Oxide");
        embeded_layers.add(oxideLayer);
        LayerInfo sulfurLayer = new LayerInfo("Sulfur");
        embeded_layers.add(sulfurLayer);
    }
    public LayerInfo info = new LayerInfo("blank");
    private List<NodeSlot> alterInputSlots = new ArrayList<>();
    private List<NodeSlot> alterOutputSlots = new ArrayList<>();
    private List<LayerInfo> embeded_layers = new ArrayList<>();
    private boolean choosing_color = false;
    @Override
    public void layout(NkContext ctx, float sx, float sy, float panelX, float panelY, boolean linking_active) {
        try(MemoryStack stack = MemoryStack.stackPush()){
            NkInput in = ctx.input();
            nk_layout_space_push(ctx,
                    NkRect.create().set( x-sx,y-sy,w,h)
            );
            if(nk_group_begin(ctx,title,NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_WINDOW_TITLE)){
                elements(ctx,sx,sy, panelX,panelY);
                nk_group_end(ctx);
            }
            drawCircle(ctx,x-sx+panelX+w-4,y-sy+panelY + 30,false,0,stack);

            for (int i=0;i<inputSlots.size();i++){
                drawCircle(ctx,x-sx+panelX-4,y-sy+panelY + inputSlots.get(i).yOffset,true,i,stack);
            }

            NkRect rect = NkRect.malloc(stack).set(panelX+x-sx,panelY+y-sy,w,h);
            if(!choosing_color && !linking_active && nk_input_is_mouse_hovering_rect(in,rect) && nk_input_is_mouse_down(in,NK_BUTTON_LEFT)){
                x += in.mouse().delta().x();
                y += in.mouse().delta().y();
            }
        }
    }
    protected ByteBuffer useEmbededLayers = GLBuffers.newDirectByteBuffer(1);
    private boolean sulfurCurveInputOccupied = false;
    protected ByteBuffer useCommonCurvatureMap = GLBuffers.newDirectByteBuffer(1);
    @Override
    protected void elements(NkContext ctx, float sx, float sy, float panelX, float panelY) {
        nk_layout_row(ctx,NK_DYNAMIC,25,new float[]{1});

        if(nk_checkbox_label(ctx,"Use EmbededLayers",useEmbededLayers)){
            inputSlots.removeAll(alterInputSlots);
            outputSlots.removeAll(alterOutputSlots);
            alterInputSlots.clear();
            alterOutputSlots.clear();
            if(useEmbededLayers.get(0) == 1){
                layerInit();
            }
        }
        int useEL = useEmbededLayers.get(0);
        if(useEL == 0){
            // diy
            info.type = "DIYLayer";
        }else{
            // default layers
            nk_layout_row(ctx,NK_STATIC,25,new float[]{50,100});
            nk_label(ctx,"name",NK_TEXT_LEFT);
            if(nk_combo_begin_label(ctx,info.name,nk_vec2(100,100, NkVec2.create()))){
                nk_layout_row(ctx,NK_DYNAMIC,25,new float[]{1});
                for (LayerInfo in: embeded_layers
                     ) {
                    if(nk_combo_item_label(ctx,in.name,NK_TEXT_LEFT)){
                        inputSlots.removeAll(alterInputSlots);
                        outputSlots.removeAll(alterOutputSlots);
                        alterInputSlots.clear();
                        alterOutputSlots.clear();
                        info.copy(in);
                        layerInit();

                    }
                }
                nk_combo_end(ctx);
            }
            if (info.name.equals("Oxide")){
                layoutOxide(ctx);
            } else if (info.name.equals("Sulfur")) {
                layoutSulfur(ctx);
            }
        }
    }
    private int curvatureSlot = -1;
    private void layerInit(){
        if(info.name.equals("Oxide")){
            Vec4 color = ColorHelper.colorOf(FilmComposition.CuII_O);
            background.set(color.x/255,color.y/255,color.z/255,color.w/255);
            info.colorf = background;
            info.type = "OxideLayer";
            info.composition = FilmComposition.CuII_O;
        }else if(info.name.equals("Sulfur")){
            Vec4 color = ColorHelper.colorOf(FilmComposition.CuSO4);
            background.set(color.x/255,color.y/255,color.z/255,color.w/255);
            info.colorf = background;
            info.type = "SulfurLayer";
            info.composition = FilmComposition.CuSO4;
            NodeSlot slot = new NodeSlot();
            WindowImage temp = new WindowImage();
            temp.id = -1;
            slot.dataType = WindowImage.class;
            slot.data = temp;
            slot.yOffset = 216;
            curvatureSlot = slot.id;
            info.imgDictionary.put("curve", temp);
            alterInputSlots.add(slot);
            inputSlots.add(slot);
        }
    }
    public NkColorf background = NkColorf.create();
    private void layoutOxide(NkContext ctx){
        {
            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx,"composition:",NK_TEXT_LEFT);
            if(nk_combo_begin_label(ctx,info.composition.toString(),nk_vec2(100,100, NkVec2.create()))){
                nk_layout_row(ctx,NK_DYNAMIC,25,new float[]{1});
                for (FilmComposition composition:FilmComposition.values()) {
                    if (nk_combo_item_label(ctx, composition.toString(), NK_TEXT_LEFT)) {
                        info.composition = composition;
                    }
                }
                nk_combo_end(ctx);
            }
            nk_label(ctx, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_combo_begin_color(ctx, NkColor.create().set((byte) (255 *background.r()), (byte) (255 *background.g()),(byte) (255 *background.b()),(byte) (255 *background.a())), nk_vec2(nk_widget_width(ctx),400,NkVec2.create()))) {
                choosing_color = true;
                nk_layout_row_dynamic(ctx, 120, 1);
                background = nk_color_picker(ctx, background, NK_RGBA);
                nk_layout_row_dynamic(ctx, 25, 1);
                background.r(nk_propertyf(ctx,"#R:",0,background.r(),1,0.01f,0.01f));
                background.g(nk_propertyf(ctx,"#G:",0,background.g(),1,0.01f,0.01f));
                background.b(nk_propertyf(ctx,"#B:",0,background.b(),1,0.01f,0.01f));
                background.a(nk_propertyf(ctx,"#A:",0,background.a(),1,0.01f,0.01f));
                nk_combo_end(ctx);
            }else {
                choosing_color = false;
            }
        }
    }
    private void layoutSulfur(NkContext ctx){
        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx,"composition:",NK_TEXT_LEFT);
        if(nk_combo_begin_label(ctx,info.composition.toString(),nk_vec2(100,100, NkVec2.create()))){
            nk_layout_row(ctx,NK_DYNAMIC,25,new float[]{1});
            for (FilmComposition composition:FilmComposition.values()) {
                if (nk_combo_item_label(ctx, composition.toString(), NK_TEXT_LEFT)) {
                    info.composition = composition;
                }
            }
            nk_combo_end(ctx);
        }
        nk_label(ctx, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 25, 1);
        if (nk_combo_begin_color(ctx, NkColor.create().set((byte) (255 *background.r()), (byte) (255 *background.g()),(byte) (255 *background.b()),(byte) (255 *background.a())), nk_vec2(nk_widget_width(ctx),400,NkVec2.create()))) {
            choosing_color = true;
            nk_layout_row_dynamic(ctx, 120, 1);
            background = nk_color_picker(ctx, background, NK_RGBA);
            nk_layout_row_dynamic(ctx, 25, 1);
            background.r(nk_propertyf(ctx,"#R:",0,background.r(),1,0.01f,0.01f));
            background.g(nk_propertyf(ctx,"#G:",0,background.g(),1,0.01f,0.01f));
            background.b(nk_propertyf(ctx,"#B:",0,background.b(),1,0.01f,0.01f));
            background.a(nk_propertyf(ctx,"#A:",0,background.a(),1,0.01f,0.01f));
            nk_combo_end(ctx);
        }else {
            choosing_color = false;
        }
        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx,"curve:",NK_TEXT_LEFT);
        if(nk_checkbox_label(ctx,"common curvature",useCommonCurvatureMap)){
            if(useCommonCurvatureMap.get(0)==1){
                NodeSlot toRemove = null;
                for (NodeSlot slot: inputSlots
                     ) {
                    if(slot.id == curvatureSlot){
                        toRemove = slot;
                        break;
                    }
                }
                if(toRemove!=null){
                    alterInputSlots.remove(toRemove);
                    inputSlots.remove(toRemove);
                }
                info.imgDictionary.put("curve",ControllTab.instance.commonCurvature);
            }else{
                NodeSlot slot = new NodeSlot();
                WindowImage temp = new WindowImage();
                temp.id = -1;
                slot.dataType = WindowImage.class;
                slot.data = temp;
                slot.yOffset = 216;
                curvatureSlot = slot.id;
                info.imgDictionary.put("curve", temp);
                alterInputSlots.add(slot);
                inputSlots.add(slot);
            }
        }
        if(useCommonCurvatureMap.get(0)==0){
            if(!sulfurCurveInputOccupied){
                FilePanel.layoutTexturePicker(ctx,info.imgDictionary.get("curve"));
            }
        }
    }
}
