package com.rust.view;

import com.jogamp.opengl.GLCapabilities;
import com.jogamp.opengl.GLProfile;
import com.jogamp.opengl.awt.GLCanvas;
import com.jogamp.opengl.util.FPSAnimator;
import com.rust.view.window.LayerInfo;
import com.rust.view.window.NodeObject;
import com.rust.view.window.NodeSlot;

import javax.swing.*;
import java.awt.*;
import java.io.FileNotFoundException;

public class TestConfig {
    public static void main(String[] args) throws FileNotFoundException {
        NodeObject.init();
        NodeSlot.init();
        try {
            new TestFrame().setup("");
        }catch (Exception e){
            e.printStackTrace();
        }
    }
}
