package com.rust.util;
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


import com.rust.view.Demo;
import com.rust.view.HeaderBar;
import com.rust.view.MainWindow;
import com.rust.view.window.ControllTab;
import com.rust.view.window.FilePanel;
import com.rust.view.window.NodeEditor;
import com.rust.view.window.StatusBar;
import com.rust.view.window.style.Style;
import io.qt.gui.QOpenGLContext;
import io.qt.gui.QOpenGLFunctions;
import io.qt.gui.QSurfaceFormat;
import io.qt.opengl.widgets.QOpenGLWidget;
import io.qt.widgets.QApplication;
import org.lwjgl.glfw.GLFW;
import org.lwjgl.glfw.GLFWCursorPosCallbackI;
import org.lwjgl.glfw.GLFWKeyCallbackI;
import org.lwjgl.system.libffi.FFICIF;
import static org.lwjgl.opengl.GL11C.*;

/**
 * @author elect
 */
public class MyFrame extends QOpenGLWidget implements GLFWKeyCallbackI, GLFWCursorPosCallbackI{
    public static int SCR_WIDTH = 800;
    public static int SCR_HEIGHT = 600;
    protected QOpenGLFunctions f;

    public StartPipelineHandler startPipelineHandler = new StartPipelineHandler();
    public RestartPipelineHandler restartPipelineHandler = new RestartPipelineHandler();
    public MyFrame(String title){
        setup(title);
    }
    public MyFrame() {
    }
    public void setup(String title) {
        QSurfaceFormat format = new QSurfaceFormat();
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        format.setVersion(3,3);
        format.setProfile(QSurfaceFormat.OpenGLContextProfile.CoreProfile);
        QSurfaceFormat.setDefaultFormat(format);
        setWindowTitle(title);
        setGeometry(0,0,SCR_WIDTH,SCR_HEIGHT);
    }
    @Override
    protected void initializeGL() {
        super.initializeGL();
        f = QOpenGLContext.currentContext().functions();
        f.glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
    }

    @Override
    protected void paintGL() {
        f = QOpenGLContext.currentContext().functions();
        f.glClearColor(0f,255f,0f,255);
        f.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        display();
    }
    protected void init(){

    }
    protected void display() {
    }
    protected void processKeyInput(long c){

    }
    protected void processMouseInput(double xpos,double ypos){

    }
    @Override
    public FFICIF getCallInterface() {
        return GLFWKeyCallbackI.super.getCallInterface();
    }
    @Override
    public void callback(long ret, long args) {
        GLFWKeyCallbackI.super.callback(ret, args);
    }
    @Override
    public void invoke(long l, int i, int i1, int i2, int i3) {
        processKeyInput(i);
    }
    @Override
    public void invoke(long l, double v, double v1) {
        processMouseInput(v,v1);
    }
}
