#include "Camera.h"
float Camera::YAW = -95;
float Camera::PITCH = 0;
float Camera::SPEED = 1;
float Camera::SENSITIVITY = 0.8f;
float Camera::ZOOM = 10;
Camera::Camera()
{
    Front = vec3(0, 0, -1);
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    Zoom = ZOOM;
    Position = vec3(0, 0, 3);
    WorldUp = vec3(0, 1, 0);
    WorldRight = vec3(1, 0, 0);
    WorldDown = vec3(0, -1, 0);
    Center = Position + Front;
    Yaw = YAW;
    Pitch = PITCH;
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    vec3 front;
    front.x = (float)(cos(radians(Yaw)) * cos(radians(Pitch)));
    front.y = (float)sin(radians(Pitch));
    front.z = (float)(sin(radians(Yaw)) * cos(radians(Pitch)));
    Front = normalize(front);
    // also re-calculate the Right and Up vector
    Right = normalize(cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = normalize(cross(Right, Front));
}
// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(double xoffset, double yoffset, bool constrainPitch)
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

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;

    if (direction == FORWARD)
        Position = Position  + Up * velocity;
    if (direction == BACKWARD)
        Position = Position - Up * velocity;
    if (direction == LEFT)
        Position = Position - Right * velocity;
    if (direction == RIGHT)
        Position = Position + Right * velocity;
}
void Camera::Move(Camera_Movement direction, float delta) {
    if (direction == UP)
        Position = Position + Up * delta;
    if (direction == DOWN)
        Position = Position - Up * delta;
    if (direction == LEFT)
        Position = Position - Right * delta;
    if (direction == RIGHT)
        Position = Position + Right * delta;
    if (direction == FORWARD)
        Position = Position + Front * delta;
    if (direction == BACKWARD)
        Position = Position - Front * delta;
}
void Camera::camera_rotate(bool isClockWise, bool isHorizontal, float delta) {
    vec4 currentDelta(Front.x, Front.y, Front.z, 1);
    currentDelta = currentDelta * 2.0f;
    vec3 temp = Position + vec3(currentDelta);
    mat4 rotate(1);
    if (isClockWise) {
        if (isHorizontal) {
            rotate = glm::rotate(rotate, radians(delta), WorldUp);
        }
        else {
            rotate = glm::rotate(rotate, radians(delta), WorldRight);
        }
        currentDelta = rotate * currentDelta;
        
        /*
        qDebug() << "r0 " << " " << rotate[0][0] << " " << rotate[0][1] << " " << rotate[0][2] << " " << rotate[0][3];
        qDebug() << "r1 " << " " << rotate[1][0] << " " << rotate[1][1] << " " << rotate[1][2] << " " << rotate[1][3];
        qDebug() << "r2 " << " " << rotate[2][0] << " " << rotate[2][1] << " " << rotate[2][2] << " " << rotate[2][3];
        qDebug() << "r3 " << " " << rotate[3][0] << " " << rotate[3][1] << " " << rotate[3][2] << " " << rotate[3][3];
        */

        Position = temp - vec3(currentDelta);
        vec4 t2(Front.x, Front.y, Front.z, 0);
        t2 = rotate * t2;
        Front = vec3(t2.x, t2.y, t2.z);
        if ((Front - WorldUp).length() > 0.1f) {
            Right = normalize(cross(Front, WorldUp));
        }
        else {
            Right = normalize(cross(Front, WorldDown));
        }
    }
    else {
        if (isHorizontal) {
            rotate = glm::rotate(rotate, radians(-delta), WorldUp);
        }
        else {
            rotate = glm::rotate(rotate, radians(-delta), WorldRight);
        }
        currentDelta = rotate * currentDelta;
        Position = temp - vec3(currentDelta);
        vec4 t2(Front.x, Front.y, Front.z, 0);
        t2 = rotate * t2;
        Front = vec3(t2);
        if ((Front-WorldUp).length() > 0.1f) {
            Right = normalize(cross(Front, WorldUp));
        }
        else {
            Right = normalize(cross(Front, WorldDown));
        }
    }
}

mat4 Camera::GetViewMatrix()
{
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    vec3 center = Position + Front;
    return lookAtRH(Position, center, Up);
}

vec3 Camera::ToObjectOffset()
{
    return Position;
}
void Camera::rotate_around(Camera_Movement axis, float delta) {
    vec3 center = Position + Front * 2.0f;
    
}
