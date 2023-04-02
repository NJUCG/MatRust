#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// material parameters
uniform bool is_pipeline_on;

uniform float i_metallic;
uniform float i_roughness;
uniform float ao;

uniform sampler2D material_texture_diffuse0;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;


// lights, max for 4
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform int lightCount;

uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float Distribution(vec3 N,vec3 half_v,float roughness){
    float ag=roughness;
    float s_ag=ag*ag;
    float cosTheta=dot(half_v,N);
    float xcosTheta=cosTheta;
    if(xcosTheta<0){
        xcosTheta=0;
    }
    float s_cosTheta=cosTheta*cosTheta;
    float s_tanTheta=(1 - s_cosTheta) / s_cosTheta;
    float d_nominator=ag*ag*xcosTheta;
    float d_denominator= PI * s_cosTheta * s_cosTheta*(s_ag+s_tanTheta)*(s_ag + s_tanTheta);

    return d_nominator/d_denominator;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);
    vec4 t = texture(material_texture_diffuse0,TexCoords);
    vec3 albedo = vec3(t.xyz);
    vec3 F0 = vec3(0.04);
    vec4 metallic4 = texture(metallicMap,TexCoords);
    vec4 roughness4 = texture(roughnessMap,TexCoords);

    float metallic = metallic4.z;
    float roughness = roughness4.z;

    if(!is_pipeline_on){
        metallic = i_metallic;
        roughness = i_roughness;
    }

    F0 = mix(F0, albedo, metallic);
    // reflectance equation
    vec3 Lo = vec3(0.0f);
    for(int i = 0; i < 4; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance    = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = lightColors[i] * attenuation;

        // cook-torrance brdf
        float NDF = Distribution(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}