#version 330 core
precision mediump float;

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in vec3 a_tangent;
layout(location = 4) in vec4 a_texBlend; 

uniform mat4 MVP;
// uniform vec3 sunDir;
// uniform mat4 shadowMatrix;

uniform vec3 planetCenter;
uniform float seaLevel;

out vec2 v_texCoords;
out vec4 v_texBlend;
out float v_y;

// out float v_shadowOpacity;

// out vec3 v_sunDirTanSpace;
// out vec3 v_toCamera;

// out vec4 shadowMapCoords;

void main() {
    gl_Position = MVP * vec4(a_pos, 1);
    v_texCoords = a_texCoords * 50;
    v_texBlend = a_texBlend;
    v_y = length(a_pos - planetCenter) - seaLevel;

    // v_texBlend = a_texBlend;
    // shadowMapCoords = shadowMatrix * vec4(a_pos, 1);

    // vec3 up = a_normal;
    // vec3 tan = a_tangent;
    // vec3 bitan = normalize(cross(up, tan));

    // mat3 toTanSpace = mat3(
    //     tan.x, bitan.x, up.x,
    //     tan.y, bitan.y, up.y,
    //     tan.z, bitan.z, up.z
    // );

    // v_sunDirTanSpace = toTanSpace * sunDir;

    // float normalSunDot = dot(normalize(a_pos), sunDir);

    // v_shadowOpacity = clamp(normalSunDot * 2., 0., 1.);

    // v_toCamera = (a_pos - (inverse(MVP) * vec4(0, 0, 0, 1)).xyz);
}
