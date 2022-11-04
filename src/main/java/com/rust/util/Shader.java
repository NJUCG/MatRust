package com.rust.util;

import com.jogamp.opengl.util.GLBuffers;
import com.sun.jna.platform.win32.OaIdl;
import glm.Glm;
import glm.mat._4.Mat4;
import glm.vec._3.Vec3;
import glm.vec._4.Vec4;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import static org.lwjgl.opengl.GL33C.*;

public class Shader {
    public static String VertexShader(String name){
        return "src/main/resources/shaders/vert/" + name + ".vert";
    }
    public static String FragShader(String name){
        return "src/main/resources/shaders/frag/" + name + ".frag";
    }
    // the program ID
    public Integer ID;
    public Shader(String name) throws FileNotFoundException {
        shared_init(VertexShader(name),FragShader(name));
    }
    // constructor reads and builds the shader
    public Shader(String vertexPath, String fragmentPath) throws FileNotFoundException {
        shared_init(vertexPath,fragmentPath);
    }
    private void shared_init(String vertexPath,String fragmentPath) throws FileNotFoundException {
        StringBuffer vertexBuffer = new StringBuffer();
        try{
            FileInputStream vertexStream = new FileInputStream(vertexPath);
            InputStreamReader vertexReader = new InputStreamReader(vertexStream,"UTF-8");
            while (vertexReader.ready()){
                vertexBuffer.append((char)vertexReader.read());
            }
            vertexReader.close();
        }catch (FileNotFoundException e){
            throw new FileNotFoundException("Vertex shader file " + vertexPath + " not found.");
        } catch (UnsupportedEncodingException e) {
            throw new RuntimeException(e);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        Integer vertShader = createShader(GL_VERTEX_SHADER,vertexBuffer.toString());

        StringBuffer fragBuffer = new StringBuffer();
        try{
            FileInputStream fragStream = new FileInputStream(fragmentPath);
            InputStreamReader fragReader = new InputStreamReader(fragStream,"UTF-8");
            while (fragReader.ready()){
                fragBuffer.append((char)fragReader.read());
            }
            fragReader.close();
        }catch (FileNotFoundException e){
            throw new FileNotFoundException("Vertex shader file " + vertexPath + " not found.");
        } catch (UnsupportedEncodingException e) {
            throw new RuntimeException(e);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        Integer fragShader = createShader(GL_FRAGMENT_SHADER,fragBuffer.toString());
        ArrayList<Integer> shaders = new ArrayList<>();
        shaders.add(vertShader);
        shaders.add(fragShader);
        ID = createProgram(shaders);
    }
    // use/activate the shader
    public void use(){
        glUseProgram(ID);
    }
    // utility uniform functions
    public void setBool(String name, Boolean value){
        int intv = value ? 1:0;
        glUniform1i(glGetUniformLocation(ID,name),intv);
    }
    public void setInt(String name, Integer value){
        glUniform1i(glGetUniformLocation(ID,name),value);
    }
    public void setFloat(String name, Float value){
        glUniform1f(glGetUniformLocation(ID,name),value);
    }
    public void setMat4(String name, Mat4 value) {
        glUniformMatrix4fv(glGetUniformLocation(ID,name),false,GLBuffers.newDirectFloatBuffer(value.toFa_()));
    }
    public void setVec3(String name, Vec3 value){
        glUniform3fv(glGetUniformLocation(ID,name),GLBuffers.newDirectFloatBuffer(value.toFa_()));
    }
    public void setVec4(String name, Vec4 value){
        glUniform3fv(glGetUniformLocation(ID,name),GLBuffers.newDirectFloatBuffer(value.toFA_()));
    }
    private int createShader(int shaderType, String shaderFile) {

        int shader = glCreateShader(shaderType);
        glShaderSource(shader, shaderFile);

        glCompileShader(shader);

        IntBuffer status = GLBuffers.newDirectIntBuffer(1);
        glGetShaderiv(shader, GL_COMPILE_STATUS, status);
        if (status.get(0) == GL_FALSE) {

            IntBuffer infoLogLength = GLBuffers.newDirectIntBuffer(1);
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, infoLogLength);

            ByteBuffer bufferInfoLog = GLBuffers.newDirectByteBuffer(infoLogLength.get(0));
            glGetShaderInfoLog(shader, infoLogLength, bufferInfoLog);
            byte[] bytes = new byte[infoLogLength.get(0)];
            bufferInfoLog.get(bytes);
            String strInfoLog = new String(bytes);
            String strShaderType = "";
            switch (shaderType) {
                case GL_VERTEX_SHADER:
                    strShaderType = "vertex";
                    break;
                case GL_GEOMETRY_SHADER:
                    strShaderType = "geometry";
                    break;
                case GL_FRAGMENT_SHADER:
                    strShaderType = "fragment";
                    break;
            }
            System.err.println("Compiler failure in " + strShaderType + " shader: " + strInfoLog);
        }
        return shader;
    }
    private int createProgram(ArrayList<Integer> shaderList) {

        int program = glCreateProgram();

        shaderList.forEach(shader -> glAttachShader(program, shader));

        glLinkProgram(program);

        IntBuffer status = GLBuffers.newDirectIntBuffer(1);
        glGetProgramiv(program, GL_LINK_STATUS, status);
        if (status.get(0) == GL_FALSE) {

            IntBuffer infoLogLength = GLBuffers.newDirectIntBuffer(1);
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, infoLogLength);

            ByteBuffer bufferInfoLog = GLBuffers.newDirectByteBuffer(infoLogLength.get(0));
            glGetProgramInfoLog(program, infoLogLength, bufferInfoLog);
            byte[] bytes = new byte[infoLogLength.get(0)];
            bufferInfoLog.get(bytes);
            String strInfoLog = new String(bytes);

            System.err.println("Linker failure: " + strInfoLog);
        }
        shaderList.forEach(shader -> glDetachShader(program, shader));
        return program;
    }
}
