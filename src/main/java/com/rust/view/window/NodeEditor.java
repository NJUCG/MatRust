package com.rust.view.window;

import com.rust.view.ILinkingResponder;
import com.rust.view.NodeCurve;
import com.rust.view.window.style.Style;
import org.lwjgl.nuklear.*;
import org.lwjgl.system.MemoryStack;

import java.util.*;

import static org.lwjgl.nuklear.Nuklear.*;

public class NodeEditor extends MyWindow implements ILinkingResponder {
    public Dictionary<Integer,Node> nodeDictionary = new Hashtable<>();
    public NodeEditor(){
        init();
    }

    public void initAfterOpenGL(){
        TextileMachine temp = TextileMachine.instance;
        temp.responder = this;
        temp.canvas = canvas;
        nodeDictionary.put(nextId(),temp);
    }
    private LinkedList<Integer> id_pool = new LinkedList<>();
    private void init(){
        for(int i=0;i<2048;i++){
            id_pool.add(i);
        }
    }
    private int nextId(){
        int ret = id_pool.poll();
        id_pool.add(ret);
        return ret;
    }
    NkCommandBuffer canvas;
    NkVec2 scrolling = NkVec2.create().set(0,0);
    NkRect total_space = NkRect.create();
    boolean linking_active;
    Node linking_start_node;
    Node linking_stop_node;
    NodeSlot linking_start_slot;
    NodeSlot linking_stop_slot;
    int linking_input_id;
    int linking_output_id;

    public void layout(NkContext ctx,int wx, int wy, int rectH, int rectW){
        seed = -1;
        try (MemoryStack stack = MemoryStack.stackPush()) {
            ctx = Style.style.controllTabStyle(ctx);
            ctx.style().contextual_button().border(1f);
            ctx.style().contextual_button().border_color(nk_rgb(255,255,255,NkColor.malloc(stack)));
            NkInput in = ctx.input();
            NkColor grey = nk_rgb(100, 100, 100,NkColor.malloc(stack));
            NkRect rect = NkRect.malloc(stack);
            if (nk_begin(
                    ctx,
                    "Node Editor",
                    nk_rect(wx, wy, rectW, rectH, rect),
                    NK_WINDOW_MOVABLE |  NK_WINDOW_MINIMIZABLE | NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_NO_SCROLLBAR
            )) {
                canvas = nk_window_get_canvas(ctx);
                nk_window_get_content_region(ctx,total_space);
                nk_layout_space_begin(ctx,NK_STATIC,total_space.h(),100000);
                float x, y;
                float grid_size = 32.0f;
                NkColor grid_color = NkColor.malloc(stack).set((byte) 50, (byte) 50,(byte)50,(byte)255);
                NkRect size = NkRect.malloc(stack);
                nk_layout_space_bounds(ctx, size);
                for (x = (float)Math.floorMod((int) (size.x() - scrolling.x()), (int) grid_size); x < size.w(); x += grid_size)
                    nk_stroke_line(canvas, x+size.x(), size.y(), x+size.x(), size.y()+size.h(), 1.0f, grid_color);
                for (y = (float)Math.floorMod((int) (size.y() - scrolling.y()), (int) grid_size); y < size.h(); y += grid_size)
                    nk_stroke_line(canvas, size.x(), y+size.y(), size.x()+size.w(), y+size.y(), 1.0f, grid_color);
                NkPanel panel;
                panel = nk_window_get_panel(ctx);
                float panelX = panel.bounds().x();
                float panelY = panel.bounds().y();
                Enumeration<Integer> t = nodeDictionary.keys();
                while (t.hasMoreElements()) {
                    Node node = nodeDictionary.get(t.nextElement());
                    node.canvas = canvas;
                    node.layout(ctx,scrolling.x(),scrolling.y(),panelX,panelY,linking_active);
                }

                {
                    if(nk_contextual_begin(ctx,0,nk_vec2(100,220,NkVec2.malloc(stack)),nk_window_get_bounds(ctx,NkRect.malloc(stack)))) {
                        nk_layout_row_dynamic(ctx,25,1);
                        if(nk_contextual_item_label(ctx,"Layer",NK_TEXT_CENTERED)){
                            LayerNode node = new LayerNode();
                            node.responder = this;
                            node.canvas = canvas;
                            nodeDictionary.put(nextId(),node);
                        }
                        nk_contextual_end(ctx);
                    }
                }

                nk_layout_space_end(ctx);
                /* draw curve from linked node slot to mouse position */
                if (linking_active) {
                    float cx = linking_start_slot.pos.x();
                    float cy = linking_start_slot.pos.y();
                    NkVec2 l0 = nk_vec2(cx + 3, cy + 3,NkVec2.malloc(stack));
                    NkVec2 l1 = in.mouse().pos();
                    nk_stroke_curve(canvas, l0.x(), l0.y(), l0.x() + 50.0f, l0.y(),
                            l1.x() - 50.0f, l1.y(), l1.x(), l1.y(), 1.0f, grey);
                }
                /* window content scrolling */
                if (!linking_active && nk_input_is_mouse_hovering_rect(ctx.input(), nk_window_get_bounds(ctx,NkRect.calloc(stack))) &&
                        nk_input_is_mouse_down(ctx.input(), NK_BUTTON_MIDDLE)) {
                    scrolling.x(scrolling.x()+ctx.input().mouse().delta().x());
                    scrolling.y(scrolling.y()+ctx.input().mouse().delta().y());
                }
                for (NodeCurve curve:linkings
                     ) {
                    NkVec2 l0 = curve.start;
                    NkVec2 l1 = curve.stop;
                    nk_stroke_curve(canvas, l0.x(), l0.y(), l0.x() + 50.0f, l0.y(),
                            l1.x() - 50.0f, l1.y(), l1.x(), l1.y(), 1.0f, grey);
                }
                nk_end(ctx);
            }

            if (linking_active && nk_input_is_mouse_released(in, NK_BUTTON_LEFT)) {
                linking_active = false;
                linking_start_node = null;
                linking_stop_node = null;
                linking_start_slot = linking_stop_slot = null;
            }

        }
    }
    public List<NodeCurve> linkings = new ArrayList<>();
    @Override
    public void startLinking(Node node, int slot) {
        if(!linking_active){
            linking_active = true;
            linking_start_node = node;
            for (NodeSlot ns: node.outputSlots
                 ) {
                if(ns.id == slot){
                    linking_start_slot = ns;
                    break;
                }
            }
            linking_output_id = slot;
        }
    }

    @Override
    public void linkingTo(Node node, Integer slot) {
        if(linking_active){
            linking_stop_node = node;
            linking_input_id = slot;
            for (NodeSlot ns: node.inputSlots
            ) {
                if(ns.id == slot){
                    linking_stop_slot = ns;
                    break;
                }
            }
            if(linking_start_node != node){
                NodeSlot output = linking_start_slot;
                if(node.useSelfLinking){
                    int[] ret = node.selfLinking(slot,output.data);
                    if(ret!=null){
                        if(ret[0]==1){
                            slot = ret[1];
                        }
                        addCurve(node, slot);
                    }
                    return;
                }
                for (NodeSlot input:node.inputSlots
                     ) {
                    if(input.id == slot && input.dataType.equals(output.dataType)){
                        input.data = output.data;
                        addCurve(node, slot);
                    }
                }
            }
        }
    }

    @Override
    public void linkingToByIndex(Node node, Integer index) {
        if(linking_active){
            linking_stop_node = node;
            int slot = node.inputSlots.get(index).id;
            linking_input_id = slot;
            if(linking_start_node != node){
                NodeSlot output = linking_start_node.outputSlots.get(linking_output_id);
                if(node.useSelfLinking){
                    int[] ret = node.selfLinking(slot,output.data);
                    if(ret!=null){
                        if(ret[0]==1){
                            slot = ret[1];
                        }
                        addCurve(node, slot);
                    }
                    return;
                }
                for (NodeSlot input:node.inputSlots
                ) {
                    if(input.id == slot && input.dataType.equals(output.dataType)){
                        input.data = output.data;
                        addCurveByIndex(node, index);
                    }
                }
            }
        }
    }

    private void addCurve(Node node, Integer slot) {
        NodeCurve curve = new NodeCurve();
        curve.stopNodeId = node.id;
        curve.stopSlotId = slot;
        for (NodeSlot s : linking_start_node.outputSlots) {
            if(linking_output_id == s.id){
                curve.start = s.pos;
                break;
            }
        }
        for (NodeSlot s : node.inputSlots) {
            if(slot == s.id){
                curve.stop = s.pos;
                break;
            }
        }
        linkings.add(curve);
    }

    private void addCurveByIndex(Node node, Integer index) {
        NodeCurve curve = new NodeCurve();
        curve.stopNodeId = node.id;
        curve.stopSlotId = node.inputSlots.get(index).id;
        curve.start = linking_start_node.outputSlots.get(linking_output_id).pos;
        curve.stop = node.inputSlots.get(curve.stopSlotId).pos;
        linkings.add(curve);
    }

    @Override
    public void delSlot(Node node, Integer slot, boolean is_start) {
        if(is_start){
            List<NodeCurve> toDel = new ArrayList<>();
            for (NodeCurve curve:linkings){
                if(curve.hasMe(node.id,slot)){
                    toDel.add(curve);
                }
            }
            for (NodeCurve curve:toDel
                 ) {
                linkings.remove(curve);
            }
        }else{
            for (NodeCurve curve:linkings
                 ) {
                if(curve.isMe(node.id,slot)){
                    linkings.remove(curve);
                    break;
                }
            }
        }
    }
}
