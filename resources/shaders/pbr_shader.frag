#version 330 core
out vec4 FragColor;

uniform sampler2D material_texture_diffuse0;
uniform sampler2D test_texture;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;
uniform sampler2D depthMap;

uniform float height_scale;

in VS_OUT{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 Normal;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 TangentLightPos;
} fs_in;

uniform vec3 lightDir;
uniform vec3 lightIntensity;
uniform vec3 viewPos;
uniform float roughness;
float fresnelSchlick(float cosTheta,float F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
void main()
{
    float pi=3.1415926;
    vec3 N=vec3(0,0,1);
    vec3 V=normalize(fs_in.TangentViewPos-fs_in.TangentFragPos);
    vec2 texCoord=fs_in.TexCoord;
    if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
        discard;

    vec3 L=normalize(fs_in.TangentLightPos-fs_in.TangentFragPos);
    vec3 half_v=normalize(L+V);

    float ag=texture(roughnessMap,texCoord);
    float s_ag=ag*ag;
    float cosTheta=dot(half_v,N);
    float xcosTheta=cosTheta;
    if(xcosTheta<0){
        xcosTheta=0;
    }
    float s_cosTheta=cosTheta*cosTheta;
    float s_tanTheta=(1 - s_cosTheta) / s_cosTheta;
    float d_nominator=ag*ag*xcosTheta;
    float d_denominator= pi * s_cosTheta * s_cosTheta*(s_ag+s_tanTheta)*(s_ag + s_tanTheta);
    float d=d_nominator/d_denominator;

    float cosV=dot(V,N);
    float s_cosV=cosV*cosV;
    float s_tanV=(1-s_cosV)/s_cosV;
    float g_nominator=2*dot(V,half_v)/cosV;
    if(g_nominator<0){
        g_nominator=0;
    }
    float g_denominator=1+sqrt(1+s_ag*s_tanV);
    float g=g_nominator/g_denominator;

    float F0 = 0.04;

    float brdf= d * g * fresnelSchlick(max(dot(half_v, V), 0.0),F0);

    float depth=texture(depthMap, fs_in.TexCoord)[0];
    FragColor = texture(material_texture_diffuse0,texCoord)* vec4(0.7,0.7,0.7,0.7);// * brdf;
    FragColor[3] = 1;
}

