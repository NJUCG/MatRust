#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<qwidget.h>
using namespace glm;

enum Camera_Movement {
    UP,
    DOWN,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    Y_AXIS,
    X_AXIS
};

class Camera
{
public:
    static float YAW;
    static float PITCH;
    static float SPEED;
    static float SENSITIVITY;
    static float ZOOM;
    // camera Attributes
    vec3 Position;
    // object center
    vec3 Center;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;
    vec3 WorldDown;
    vec3 WorldRight;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
	Camera();
    void updateCameraVectors();
    void ProcessMouseMovement(double xoffset, double yoffset, bool constrainPitch);
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void Move(Camera_Movement direction, float delta);
    mat4 GetViewMatrix();
    // 获取此刻物体应该在的位置
    vec3 ToObjectOffset();
    void rotate_around(Camera_Movement, float delta);
    void camera_rotate(bool isClockWise, bool isHorizontal, float deltaTime);
private:
};

