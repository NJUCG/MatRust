package com.rust.view;

import com.jogamp.opengl.util.GLBuffers;
import com.rust.model.PipelineManager;
import com.rust.model.PipelineManagerDelegate;
import com.rust.model.PipelineState;
import com.rust.util.*;
import com.rust.viewModel.ControllPanelViewModel;
import glm.Glm;
import glm.mat._4.Mat4;
import glm.vec._3.Vec3;
import glm.vec._4.Vec4;
import org.lwjgl.glfw.GLFW;

import java.nio.DoubleBuffer;
import java.nio.IntBuffer;

import static com.rust.util.Camera_Movement.*;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL33C.*;

// 界面布局
// 分解成物理环境配置 渲染环境配置
// 资源面包 + 模型切换 背景
// 状态栏取消，改为底部播放器
// 褶皱

public class TestFrame extends MyFrame{
    private final Vec3 displayLoc = new Vec3(0,-2,-3);
    private final int metallicIndex = 8;
    private final int roughnessIndex = 9;
    Shader shader;
    Camera camera = new Camera();
    //ControllPanel controllPanel = new ControllPanel();
    Model testModel = new Model();
    IntBuffer vao = GLBuffers.newDirectIntBuffer(1);

    float[] vertices = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };
    DoubleBuffer xpos = GLBuffers.newDirectDoubleBuffer(1),ypos = GLBuffers.newDirectDoubleBuffer(1);
    int mMap,rMap;
    @Override
    protected void init()  {
        try{
            shader = new Shader("common_shader");
        }catch (Exception e){
            e.printStackTrace();
        }
        Vec4[][] metallic = new Vec4[200][200];
        Vec4[][] roughness = new Vec4[200][200];
        for(int i=0;i<200;i++){
            for(int j=0;j<200;j++){
                metallic[i][j] = new Vec4(0,0,0.8 * 255,0);
                roughness[i][j] = new Vec4(0,0,0.2 * 255,0);
            }
        }
        try {
            mMap = PipelineManager.shared.bind4Map(metallic,200,200);
            // mMap = Model.TextureFromFile("iron_metallic.png","src/main/resources/models/stranger").get(0);
        }catch (Exception e){
            e.printStackTrace();
        }
        try {
            rMap = PipelineManager.shared.bind4Map(roughness,200,200);
            // rMap = Model.TextureFromFile("iron_roughness.png","src/main/resources/models/stranger").get(0);
        }catch (Exception e){
            e.printStackTrace();
        }

        camera.Position = new Vec3(0,0,3);
        camera.Center = displayLoc;
        lastX = xpos.get(0);
        lastY = ypos.get(0);
        try {
            testModel.load("src/main/resources/models/99-intergalactic_spaceship-obj/Intergalactic_Spaceship-(Wavefront).obj","src/main/resources/models/99-intergalactic_spaceship-obj");
        }catch (Exception e){
            e.printStackTrace();
        }
        // 0.888 0.524 0.473
        glEnable(GL_DEPTH_TEST);
    }
    @Override
    protected void display() {
        float currentTime = (float)GLFW.glfwGetTime();
        TimeManager.shared.deltaTime = currentTime - TimeManager.shared.lastFrame;
        TimeManager.shared.lastFrame = currentTime;
        shader.use();
        Mat4 projection = Glm.perspective_(GlmExtension.radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        Mat4 view = camera.GetViewMatrix();
        glActiveTexture(GL_TEXTURE0 + metallicIndex);
        shader.setInt("metallicMap",metallicIndex);
        if(PipelineManager.shared.output.metallic_map<0){
            glBindTexture(GL_TEXTURE_2D, mMap);
        }else{
            glBindTexture(GL_TEXTURE_2D, PipelineManager.shared.output.metallic_map);
        }
        glActiveTexture(GL_TEXTURE0 + roughnessIndex);
        shader.setInt("roughnessMap",roughnessIndex);
        if(PipelineManager.shared.output.roughness_map<0){
            glBindTexture(GL_TEXTURE_2D, rMap);
        }else{
            glBindTexture(GL_TEXTURE_2D, PipelineManager.shared.output.roughness_map);
        }
        glActiveTexture(GL_TEXTURE0);
        shader.setFloat("ao",1.0f);
        shader.setMat4("projection", projection);
        shader.setMat4("view",view);
        shader.setVec3("camPos", camera.Position);
        // render the loaded model
        Mat4 model = new Mat4(1.0f);
        model.translate(displayLoc); // translate it down so it's at the center of the scene
        model.scale(new Vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        model.rotate(GlmExtension.radians(-90),0,1,0);
        shader.setMat4("model", model);
        setUpLight();
        testModel.Draw(shader);
        processInput();
        ControllPanelViewModel.shared.frameFun();
        PipelineManager.shared.frameFun();
    }

    private void setUpLight(){
        shader.setInt("lightCount", 4);

        Vec3 lightPos = new Vec3(0, 0, displayLoc.z + 3);
        shader.setVec3("lightPositions[0]", lightPos);

        int t = 16;
        Vec3 lightIntensity = new Vec3(t, t, t);
        shader.setVec3("lightColors[0]", lightIntensity);

        lightPos = new Vec3(0, 0, displayLoc.z - 3);
        shader.setVec3("lightPositions[1]", lightPos);

        shader.setVec3("lightColors[1]", lightIntensity);

        lightPos = new Vec3(displayLoc.x - 2, 0, displayLoc.z);
        shader.setVec3("lightPositions[2]", lightPos);

        shader.setVec3("lightColors[2]", lightIntensity);

        lightPos = new Vec3(displayLoc.x + 2, 0, displayLoc.z);
        shader.setVec3("lightPositions[3]", lightPos);

        shader.setVec3("lightColors[3]", lightIntensity);
    }

    boolean changeView = false;
    private double lastX = 0;
    private double lastY = 0;

    private boolean turnHeadEnabled = false;
    public void processInput() {
        /*camera.FrameFun();
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
            changeView = true;
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, TimeManager.shared.deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, TimeManager.shared.deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, TimeManager.shared.deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, TimeManager.shared.deltaTime);
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
            turnHeadEnabled = !turnHeadEnabled;
            if(turnHeadEnabled){
                GLFW.glfwGetCursorPos(window,xpos,ypos);
                lastX = xpos.get(0);
                lastY = ypos.get(0);
            }
        }

        if(turnHeadEnabled){
            GLFW.glfwGetCursorPos(window,xpos,ypos);
            camera.ProcessMouseMovement(xpos.get(0)-lastX,ypos.get(0)-lastY,true);
            lastX = xpos.get(0);
            lastY = ypos.get(0);
        }*/
    }

}
