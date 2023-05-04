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
        Position = Position  + WorldUp * velocity;
    if (direction == BACKWARD)
        Position = Position - WorldUp * velocity;
    if (direction == LEFT)
        Position = Position - WorldRight * velocity;
    if (direction == RIGHT)
        Position = Position + WorldRight * velocity;
}
void Camera::Move(Camera_Movement direction, float delta) {
    if (direction == UP)
        Position = Position + WorldUp * delta;
    if (direction == DOWN)
        Position = Position - WorldUp * delta;
    if (direction == LEFT)
        Position = Position - WorldRight * delta;
    if (direction == RIGHT)
        Position = Position + WorldRight * delta;
    if (direction == FORWARD)
        Position = Position + Front * delta;
    if (direction == BACKWARD)
        Position = Position - Front * delta;
}
void Camera::camera_rotate(bool isClockWise, bool isHorizontal, float delta) {
    /*
    vec4 currentDelta(Front.x, Front.y, Front.z, 0);
    currentDelta = currentDelta * 2.0f;
    currentDelta.z = 1.0f;
    vec3 temp = Position + vec3(currentDelta);
    mat4 rotate(1);
    if (isClockWise) {
        if (isHorizontal) {
            rotate = glm::rotate(rotate, radians(delta), Up);
        }
        else {
            rotate = glm::rotate(rotate, radians(delta), Right);
        }
        currentDelta = rotate * currentDelta;
        Position = temp - vec3(currentDelta);
        vec4 t2(Front.x, Front.y, Front.z, 0);
        t2 = rotate * t2;
        Front = vec3(t2.x, t2.y, t2.z);
        if ((Front - WorldUp).length() > 0.1f) {
            Right = normalize(cross(Front, Up));
        }
        else {
            Right = normalize(cross(Front, -Up));
        }
    }
    else {
        if (isHorizontal) {
            rotate = glm::rotate(rotate, radians(-delta), Up);
        }
        else {
            rotate = glm::rotate(rotate, radians(-delta), Right);
        }
        currentDelta = rotate * currentDelta;
        Position = temp - vec3(currentDelta);
        vec4 t2(Front.x, Front.y, Front.z, 0);
        t2 = rotate * t2;
        Front = vec3(t2);
        if ((Front-WorldUp).length() > 0.1f) {
            Right = normalize(cross(Front, Up));
        }
        else {
            Right = normalize(cross(Front, -Up));
        }
    }
    */
    mat4 rotate(1);
    if (isClockWise) {
        if (isHorizontal) {
            rotate = glm::rotate(rotate, radians(delta), Up);
            vec4 temp = vec4(Right, 0);
            Right = rotate * temp;
            temp = vec4(Front, 0);
            Front = rotate * temp;
        }
        else {
            rotate = glm::rotate(rotate, radians(delta), Right);
            vec4 temp = vec4(Up, 0);
            Up = rotate * temp;
            temp = vec4(Front, 0);
            Front = rotate * temp;
        }
    }
    else {
        if (isHorizontal) {
            rotate = glm::rotate(rotate, radians(-delta), Up);
            vec4 temp = vec4(Right, 0);
            Right = rotate * temp;
            temp = vec4(Front, 0);
            Front = rotate * temp;
        }
        else {
            rotate = glm::rotate(rotate, radians(-delta), Right);
            vec4 temp = vec4(Up, 0);
            Up = rotate * temp;
            temp = vec4(Front, 0);
            Front = rotate * temp;
        }
    }
    //qDebug() << Up.x << "," << Up.y << "," << Up.z << " / " << Right.x << "," << Right.y << "," << Right.z;
    //updateCameraVectors();
}

mat4 Camera::GetViewMatrix()
{
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