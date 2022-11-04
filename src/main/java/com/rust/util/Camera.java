package com.rust.util;

import glm.Glm;
import glm.mat._4.Mat4;
import glm.vec._3.Vec3;
import glm.vec._4.Vec4;

import static java.lang.Math.cos;
import static java.lang.Math.sin;

public class Camera {
    public static float YAW = -90.0f;
    public static float PITCH = 0.0f;
    public static float SPEED = 100.0f;
    public static float SENSITIVITY = 0.8f;
    public static float ZOOM = 45.0f;
    // camera Attributes
    public Vec3 Position;
    // object center
    public Vec3 Center;
    public Vec3 Front;
    public Vec3 Up;
    public Vec3 Right;
    public Vec3 WorldUp;
    public Vec3 WorldDown;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    public float Zoom;
    public Camera(){
        Front = new Vec3(0,0,-1);
        MovementSpeed = SPEED;
        MouseSensitivity = SENSITIVITY;
        Zoom = ZOOM;
        Position = new Vec3(0,0,0);
        WorldUp = new Vec3(0,1,0);
        WorldDown = new Vec3(0,-1,0);
        Center = new Vec3(0,0,0);
        Yaw = YAW;
        Pitch = PITCH;
        updateCameraVectors();
    }
    // constructor with vectors
    public Camera(Vec3 position, Vec3 up, float yaw, float pitch)
    {
        Front = new Vec3(0,0,-1);
        MovementSpeed = SPEED;
        MouseSensitivity = SENSITIVITY;
        Zoom = ZOOM;
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    public Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    {
        Front = new Vec3(0,0,-1);
        MovementSpeed = SPEED;
        MouseSensitivity = SENSITIVITY;
        Zoom = ZOOM;
        Position = new Vec3(posX, posY, posZ);
        WorldUp = new Vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    public Mat4 GetViewMatrix()
    {
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        Vec3 center = GlmExtension.add(Position,Front);
        return GlmExtension.lookAtRH(Position,center,Up);
    }
    public void FrameFun(){
        clocker += TimeManager.shared.deltaTime;
    }
    private float clocker = 0f;
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    public void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == Camera_Movement.FORWARD)
            camera_rotate(true,false,deltaTime);
        if (direction == Camera_Movement.BACKWARD)
            camera_rotate(false,false,deltaTime);
        if (direction == Camera_Movement.LEFT)
            camera_rotate(true,true,deltaTime);
        if (direction == Camera_Movement.RIGHT)
            camera_rotate(false,true,deltaTime);/*
        float velocity = MovementSpeed * deltaTime / 10;
        if (direction == Camera_Movement.FORWARD)
            Position = GlmExtension.sub(Position,new Vec3(0,0,velocity));
        if (direction == Camera_Movement.BACKWARD)
            Position = GlmExtension.add(Position,new Vec3(0,0,velocity));
        if (direction == Camera_Movement.LEFT)
            Position = GlmExtension.sub(Position,new Vec3(velocity,0,0));
        if (direction == Camera_Movement.RIGHT)
            Position = GlmExtension.add(Position,new Vec3(velocity,0,0));*/
    }
    private void camera_rotate(boolean isClockWise, boolean isHorizontal , float deltaTime){
        float delta = MovementSpeed * deltaTime;
        Vec3 t = GlmExtension.sub(Position,Center);
        Vec4 currentDelta = new Vec4(t.x,t.y,t.z,1);
        Mat4 rotate = new Mat4(1);
        if(isClockWise){
            if (isHorizontal){
                rotate.rotate(GlmExtension.radians(delta),0,1,0);
            }else{
                rotate.rotate(GlmExtension.radians(delta),Right.x,Right.y,Right.z);
            }
            currentDelta = rotate.mul(currentDelta);
            Position = GlmExtension.add(Center , new Vec3(currentDelta.x,currentDelta.y,currentDelta.z));
            Vec4 t2 = new Vec4(Front.x,Front.y,Front.z,0);
            t2 = rotate.mul(t2);
            Front = new Vec3(t2.x,t2.y,t2.z);
            if (GlmExtension.sub(Front,WorldUp).length()>0.1f){
                Right = Glm.cross_(Front, WorldUp).normalize();
            }else{
                Right = Glm.cross_(Front, WorldDown).normalize();
            }
        }else {
            if(isHorizontal){
                rotate.rotate(GlmExtension.radians(-delta),0,1,0);
            }else{
                rotate.rotate(GlmExtension.radians(-delta),Right.x,Right.y,Right.z);
            }
            currentDelta = rotate.mul(currentDelta);
            Position = GlmExtension.add(Center , new Vec3(currentDelta.x,currentDelta.y,currentDelta.z));
            Vec4 t2 = new Vec4(Front.x,Front.y,Front.z,0);
            t2 = rotate.mul(t2);
            Front = new Vec3(t2.x,t2.y,t2.z);
            if (GlmExtension.sub(Front,WorldUp).length()>0.1f){
                Right = Glm.cross_(Front, WorldUp).normalize();
            }else{
                Right = Glm.cross_(Front, WorldDown).normalize();
            }
        }
    }
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    public void ProcessMouseMovement(double xoffset, double yoffset, boolean constrainPitch)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
        Yaw += xoffset;
        Pitch += yoffset;
        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    public void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }
    // calculates the front vector from the Camera's (updated) Euler Angles
    public void updateCameraVectors()
    {
        // calculate the new Front vector
        Vec3 front = new Vec3();
        front.x = (float) (cos(radians(Yaw)) * cos(radians(Pitch)));
        front.y = (float) sin(radians(Pitch));
        front.z = (float) (sin(radians(Yaw)) * cos(radians(Pitch)));
        Front = front.normalize_();
        // also re-calculate the Right and Up vector
        Right = Glm.cross_(Front, WorldUp).normalize();  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = Glm.cross_(Right, Front).normalize();
    }
    public static double radians(double degrees){
        return degrees * 0.01745329251994329576923690768489;
    }
}
