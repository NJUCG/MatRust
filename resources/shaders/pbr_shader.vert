#version 330 core
#extension GL_ARB_separate_shader_objects : enable
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 Normal;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 TangentLightPos;
} vs_out;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D height_map;
uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{
    gl_Position = projection * view *  model * vec4(aPos.x,aPos.y,aPos.z, 1.0);
    vs_out.FragPos=vec3(model*vec4(aPos,1.0));
    vs_out.TexCoord=aTexCoord;
    vs_out.Normal=aNormal;

    vec3 T   = normalize(mat3(model) * aTangent);
    vec3 B   = normalize(mat3(model) * aBitangent);
    vec3 N   = normalize(mat3(model) * aNormal);
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.TangentViewPos=TBN * viewPos;
    vs_out.TangentFragPos=TBN * vs_out.FragPos;
    vs_out.TangentLightPos=TBN * lightPos;
}