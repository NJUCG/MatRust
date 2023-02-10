#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 WorldPos;
out vec3 nearPoint;
out vec3 farPoint;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

vec3 UnprojectPoint(float x, float y, float z){
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint = viewInv * projInv * vec4(x,y,z,1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

// normal vertice projection
void main() {
    nearPoint = UnprojectPoint(aPos.x, aPos.y, 0);

    farPoint = UnprojectPoint(aPos.x, aPos.y, 1);

    gl_Position = vec4(aPos,1.0);
}
