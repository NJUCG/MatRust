package com.rust.view.window;

// 引用传递的string
public class NodeString {
    public String s;
    public NodeString(String s){
        this.s = s;
    }
    public NodeString(NodeString ns){
        s = ns.s;
    }

    @Override
    public String toString() {
        return s;
    }
}
