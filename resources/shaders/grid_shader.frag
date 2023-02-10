#version 330 core
in vec3 WorldPos;
in vec3 nearPoint;
in vec3 farPoint;
uniform mat4 view;
uniform mat4 projection;
uniform float near;
uniform float far;

out vec4 FragColor;

vec4 grid(vec3 fragPos3D, float scale) {
    vec2 coord = fragPos3D.xz * scale; // use the scale variable to set the distance between the lines
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    // z axis
    if(fragPos3D.x > -1 * minimumx && fragPos3D.x < 1 * minimumx){
        color.z = 1.0;
    }
    // x axis
    if(fragPos3D.z > -1 * minimumz && fragPos3D.z < 1 * minimumz){
        color.x = 1.0;
    }
    return color;
}
float computeDepth(vec3 pos) {
    vec4 clip_space_pos = projection * view * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}
float computeLinearDepth(vec3 pos) {
    vec4 clip_space_pos = projection * view * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between 0.01 and 100
    return linearDepth / far; // normalize
}
void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);

    float a = 0;
    if(t < 0){
        discard;
    }

    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));

    gl_FragDepth = computeDepth(fragPos3D);

    FragColor = grid(fragPos3D, 1);
    FragColor.a *= fading;
}