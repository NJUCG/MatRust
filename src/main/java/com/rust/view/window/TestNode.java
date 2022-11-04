package com.rust.view.window;

import org.lwjgl.nuklear.NkContext;

import static org.lwjgl.nuklear.Nuklear.*;

public class TestNode extends Node{
    public TestNode(){
        h=w=120;
        title="title";
        x=y=50;
        NodeSlot temp = new NodeSlot();
        temp.dataType = NodeString.class;
        temp.data = word;
        outputSlots.add(temp);

        NodeSlot temp2 = new NodeSlot();
        temp2.dataType = NodeString.class;
        temp2.data = word;
        inputSlots.add(temp2);
    }
    public final NodeString word = new NodeString("hello");
    @Override
    protected void elements(NkContext ctx, float sx, float sy, float panelX, float panelY) {
        nk_layout_row(ctx,NK_DYNAMIC,25,new float[]{1});
        nk_label(ctx,getWord(),NK_TEXT_LEFT);
    }
    public String getWord(){
        return ((NodeString)inputSlots.get(2).data).s;
    }
    public void setWord(String s){
        ((NodeString)inputSlots.get(2).data).s = s;
    }
}
