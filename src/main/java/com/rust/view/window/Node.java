package com.rust.view.window;

import com.rust.view.ILinkingResponder;
import org.lwjgl.nuklear.*;
import org.lwjgl.system.MemoryStack;

import java.util.ArrayList;
import java.util.List;

import static org.lwjgl.nuklear.Nuklear.*;

public class Node {
    public int id;
    public float x,y,h,w;
    public String title;
    public NkCommandBuffer canvas;
    public List<NodeSlot> inputSlots = new ArrayList<>();
    public List<NodeSlot> outputSlots = new ArrayList<>();

    public Node(){

    }
    public void layout(NkContext ctx, float sx, float sy,float panelX,float panelY,boolean linking_active){
        try(MemoryStack stack = MemoryStack.stackPush()){
            NkInput in = ctx.input();
            nk_layout_space_push(ctx,
                    NkRect.create().set( x-sx,y-sy,w,h)
                    );
            if(nk_group_begin(ctx,title,NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_WINDOW_TITLE)){
                elements(ctx,sx,sy, panelX,panelY);
                nk_group_end(ctx);
            }
            NkRect rect = NkRect.malloc(stack).set(panelX+x-sx,panelY+y-sy,w,h);
            if(!linking_active && nk_input_is_mouse_hovering_rect(in,rect) && nk_input_is_mouse_down(in,NK_BUTTON_LEFT)){
                x += in.mouse().delta().x();
                y += in.mouse().delta().y();
            }
        }
    }
    protected void elements(NkContext ctx, float sx, float sy, float panelX, float panelY){

    }
    public ILinkingResponder responder;
    protected void drawCircle(NkContext ctx, float px, float py, boolean input, int index, MemoryStack stack){
        NkColor color = NkColor.malloc(stack).set((byte)100,(byte)100,(byte) 100,(byte) 255);
        NkRect rect = NkRect.malloc(stack).set(px, py,8,8);
        if(input){
            inputSlots.get(index).pos.set(px,py);
        }else {
            outputSlots.get(index).pos.set(px,py);
        }
        nk_fill_circle(canvas,rect,color);
        NkInput in = ctx.input();
        if (!input && nk_input_has_mouse_click_down_in_rect(in, NK_BUTTON_LEFT, rect,true)) {
            responder.startLinking(this, outputSlots.get(index).id);
        }
        if(input && nk_input_is_mouse_released(in,NK_BUTTON_LEFT) && nk_input_is_mouse_hovering_rect(in,rect)){
            responder.linkingTo(this, inputSlots.get(index).id);
        }
    }
    protected void drawCircleId(NkContext ctx, float px, float py, boolean input, int id, MemoryStack stack){
        NkColor color = NkColor.malloc(stack).set((byte)100,(byte)100,(byte) 100,(byte) 255);
        NkRect rect = NkRect.malloc(stack).set(px, py,8,8);
        if(input){
            for (NodeSlot slot: inputSlots
                 ) {
                if(slot.id == id){
                    slot.pos.set(px,py);
                    break;
                }
            }
        }else {
            for (NodeSlot slot: outputSlots
            ) {
                if(slot.id == id){
                    slot.pos.set(px,py);
                    break;
                }
            }
        }
        nk_fill_circle(canvas,rect,color);
        NkInput in = ctx.input();
        if (!input && nk_input_has_mouse_click_down_in_rect(in, NK_BUTTON_LEFT, rect,true)) {
            responder.startLinking(this, id);
        }
        if(input && nk_input_is_mouse_released(in,NK_BUTTON_LEFT) && nk_input_is_mouse_hovering_rect(in,rect)){
            responder.linkingTo(this, id);
        }
    }
    public boolean useSelfLinking = false;
    // 第一个为是否需要更改id,为1时更改为第二个
    public int[] selfLinking(Integer inputSlot, Object data){
        return null;
    }

}
