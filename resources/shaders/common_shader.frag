#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in mat3 TBN;

// material parameters
uniform bool is_pipeline_on;

uniform float i_metallic;
uniform float i_roughness;
uniform float ao;

uniform sampler2D material_texture_diffuse0;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D depthMap;
uniform float heightScale;
uniform bool use_depth;

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
    /*
    float ag=roughness;
    float s_ag=ag*ag;
    float cosTheta= dot(half_v,N);
    float xcosTheta=cosTheta;
    if(xcosTheta < 0){
        xcosTheta = abs(xcosTheta);
    }
    float s_cosTheta=cosTheta*cosTheta;
    float s_tanTheta=(1 - s_cosTheta) / s_cosTheta;
    float d_nominator=ag*ag*xcosTheta;
    float d_denominator= PI * s_cosTheta * s_cosTheta*(s_ag+s_tanTheta)*(s_ag + s_tanTheta);
    */
    float ag=roughness * roughness;
    float s_ag=ag*ag;
    float cosTheta= abs(dot(half_v, N));//max(dot(half_v, N),0.0); 
    float s_cosTheta= cosTheta*cosTheta;

    float d_nominator= s_ag;
    float d_deno_2 = s_cosTheta * (s_ag - 1) + 1;
    float d_denominator= PI *  d_deno_2 * d_deno_2;
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

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    return currentTexCoords;
}


void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);
    vec2 texCoords;
    vec3 t_camPos = TBN * camPos;
    vec3 t_worldPos = TBN * WorldPos;
    if(use_depth){
        V = normalize(t_camPos - t_worldPos);
        N = TBN * N;
        texCoords = ParallaxMapping(TexCoords, V);
        if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
            discard;
    }else{
        texCoords = TexCoords;
    }

    vec4 t = texture(material_texture_diffuse0,texCoords);
    vec3 albedo = vec3(t.xyz);
    vec3 F0 = vec3(0.04);

    vec4 metallic4 = texture(metallicMap,texCoords);
    vec4 roughness4 = texture(roughnessMap,texCoords);

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
        vec3 L;
        if(use_depth){
            L = normalize(TBN * lightPositions[i] - t_worldPos);
        }else{
            L = normalize(lightPositions[i] - WorldPos);
        }
        vec3 H = normalize(V + L);
        float distance;
        if(use_depth){
            distance = length(TBN * lightPositions[i] - t_worldPos);
        }else{
            distance = length(lightPositions[i] - WorldPos);
        }
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = lightColors[i] * attenuation;

        // cook-torrance brdf
        float NDF = Distribution(N, H, roughness);
        float G   = 1;//GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(clamp(dot(H, V),0.0,1.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator    = NDF * G * F;

        float nvDot = clamp(abs(dot(N,V)), 0.0, 1.0);//max(dot(N,V),0.0);//clamp(dot(N,V), 0.0, 1.0);
        float nlDot = clamp(abs(dot(N,V)), 0.0, 1.0);//max(dot(N,L),0.0);//clamp(dot(N,L), 0.0, 1.0);

        float denominator = 4.0 * nvDot * nlDot + 0.0001;
        vec3 specular = clamp(numerator, 0.0, 1.0);

        // add to outgoing radiance Lo
        float NdotL = max(abs(dot(N, L)), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
        //Lo += specular;
        //Lo = vec3(denominator);
        if(dot(N,V) <= 0){
            Lo = vec3(1.0);
        }else{
            Lo = vec3(0.0);
        }
        Lo = V;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = Lo + ambient;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}