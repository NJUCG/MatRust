package com.rust.view.window;

import org.lwjgl.nuklear.NkContext;
import org.lwjgl.nuklear.NkInput;
import org.lwjgl.nuklear.NkRect;
import org.lwjgl.system.MemoryStack;

import java.util.ArrayList;
import java.util.Enumeration;
import java.util.LinkedList;
import java.util.List;

import static org.lwjgl.nuklear.Nuklear.*;

public class TextileMachine extends Node{
    public static TextileMachine instance = new TextileMachine();
    private TextileMachine(){
        x = y = 200;
        h = 400;
        w = 300;
        title = "Textile Machine";
        useSelfLinking = true;
    }

    float layerSpacing = 37.5f;
    @Override
    public void layout(NkContext ctx, float sx, float sy, float panelX, float panelY, boolean linking_active) {
        try(MemoryStack stack = MemoryStack.stackPush()){
            NkInput in = ctx.input();
            nk_layout_space_push(ctx,
                    NkRect.create().set( x-sx,y-sy,w,h)
            );
            if(nk_group_begin(ctx,title,NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_COLOR_BORDER | NK_WINDOW_TITLE)){
                elements(ctx,sx,sy, panelX,panelY);
                nk_group_end(ctx);
            }
            for(int i=0;i<inputSlots.size();i++){
                drawCircle(ctx,x-sx+panelX-4,y-sy+panelY + 85 + i * layerSpacing,true,i,stack);
            }
            NkRect rect = NkRect.malloc(stack).set(panelX+x-sx,panelY+y-sy,w,h);
            if(!linking_active && nk_input_is_mouse_hovering_rect(in,rect) && nk_input_is_mouse_down(in,NK_BUTTON_LEFT)){
                x += in.mouse().delta().x();
                y += in.mouse().delta().y();
            }
        }
    }
    public List<LayerInfo> infoList = new ArrayList<>();
    @Override
    protected void elements(NkContext ctx, float sx, float sy, float panelX, float panelY) {
        nk_layout_row(ctx,NK_DYNAMIC,20,new float[]{0.2f,0.6f,0.2f});
        nk_spacing(ctx,1);
        if(nk_button_label(ctx,"add layer")){
            LayerInfo laye = new LayerInfo("blank");
            addLayer(laye);
        }
        for(int i=0;i<infoList.size();i++){
            drawLayer(ctx,i);
        }
    }
    private void drawLayer(NkContext ctx, int i){
        LayerInfo info = infoList.get(i);
        nk_layout_row(ctx,NK_DYNAMIC,32.5f,new float[]{1});
        if(nk_group_begin(ctx,"",NK_WINDOW_BORDER | NK_COLOR_BORDER | NK_WINDOW_NO_SCROLLBAR)){
            nk_layout_row(ctx,NK_DYNAMIC,20,new float[]{0.25f,0.02f,0.2f,0.02f,0.2f,0.02f,0.2f,0.09f});
            nk_label(ctx,info.name,NK_TEXT_LEFT);
            nk_spacing(ctx,1);
            if(nk_button_label(ctx,"lower")){
                if(i>0){
                    LayerInfo temp = infoList.get(i-1);
                    infoList.set(i-1,info);
                    infoList.set(i,temp);

                    NodeSlot temp_slot = inputSlots.get(i-1);
                    NodeSlot temp2_slot = inputSlots.get(i);
                    inputSlots.set(i-1, temp2_slot);
                    inputSlots.set(i,temp_slot);
                }
            }
            nk_spacing(ctx,1);
            if(nk_button_label(ctx,"upper")){
                if(i<infoList.size()-1){
                    LayerInfo temp = infoList.get(i+1);
                    infoList.set(i+1,info);
                    infoList.set(i,temp);

                    NodeSlot temp_slot = inputSlots.get(i+1);
                    NodeSlot temp2_slot = inputSlots.get(i);
                    inputSlots.set(i+1, temp2_slot);
                    inputSlots.set(i,temp_slot);
                }
            }
            nk_spacing(ctx,1);
            if(nk_button_label(ctx,"del")){
                delLayer(i);
            }
            nk_spacing(ctx,1);
            nk_group_end(ctx);
        }
    }
    private void addLayer(LayerInfo info){
        if(infoList.size()>7){
            return;
        }
        NodeSlot slot = new NodeSlot();
        slot.dataType = LayerInfo.class;
        slot.data = info;
        inputSlots.add(slot);
        infoList.add(info);
    }
    private void delLayer(int index){
        infoList.remove(inputSlots.get(index).data);
        inputSlots.remove(index);
    }
    public static int seed = -1;
    private int nextSeed(){
        seed = seed + 1;
        return seed;
    }

    @Override
    public int[] selfLinking(Integer inputSlot, Object data) {
        if(!data.getClass().equals(LayerInfo.class)){
            return null;
        }
        LayerInfo currentInfo = null;
        for (NodeSlot slot:inputSlots) {
            if(slot.id == inputSlot){
                currentInfo = (LayerInfo) slot.data;
            }
        }
        if (currentInfo==null){
            return null;
        }
        for(int i=0;i<infoList.size();i++){
            if(infoList.get(i).equals(currentInfo)){
                LayerInfo info = (LayerInfo) data;
                infoList.set(i,info);
                for (NodeSlot nodeSlot: inputSlots
                     ) {
                    if(nodeSlot.id == inputSlot){
                        nodeSlot.data = info;
                        break;
                    }
                }
                return new int[]{0};
            }
        }
        return null;
    }
}
