package com.rust.view.window;

import java.util.LinkedList;

public class NodeObject {
    public int id;
    public NodeObject(){
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
