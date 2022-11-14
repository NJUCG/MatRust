package com.rust.view;

import org.lwjgl.nuklear.NkContext;

public class MainWindow {
    Integer SCR_WIDTH;
    Integer SCR_HEIGHT;
    public void initAfterOpenGL(Integer w,Integer h){
        SCR_WIDTH = w;
        SCR_HEIGHT = h;
    }
    public void layout(NkContext ctx){

    }
}
