package com.rust.view.window;

import org.lwjgl.nuklear.NkVec2;

import java.lang.reflect.Type;
import java.util.LinkedList;

public class NodeSlot {
    public boolean occupied;
    public boolean input;
    public String name;
    public Type dataType;
    public int id;
    public Object data;
    public NkVec2 pos = NkVec2.create();
    public float yOffset;
    public NodeSlot(){
        id = nextId();
    }
    private static LinkedList<Integer> id_pool = new LinkedList<>();
    public static void init(){
        for(int i=0;i<2048;i++){
            id_pool.add(i);
        }
    }
    protected static int nextId(){
        int ret = id_pool.poll();
        id_pool.add(ret);
        return ret;
    }
}
