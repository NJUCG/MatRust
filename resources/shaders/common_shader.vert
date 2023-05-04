#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out mat3 TBN;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal_matrix;
uniform bool use_normal_map;
uniform bool use_disturb;
uniform sampler2D material_texture_normal0;
uniform sampler2D normal_disturb_map;

float random(float x){
    float y = fract(sin(x) * 1000.0f);
    return y;
}

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));

    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N;

    if(use_normal_map){
               Normal = texture(material_texture_normal0, TexCoords).xyz;
               N = normalize(mat3(model) * Normal);
               Normal = normal_matrix * normalize(Normal * 2.0 -1.0);
    }else{
             N = normalize(mat3(model) * aNormal);
             Normal = mat3(model) * aNormal;
        
    }
    TBN = transpose(mat3(T, B, N));
    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}

