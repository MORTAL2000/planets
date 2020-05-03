#version 330 core
precision mediump float;

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 4) in float a_y;

uniform mat4 MVP;
uniform vec3 sunDir;

out vec3 v_normal;
out vec2 v_texCoord;
out float v_y;

void main() {
    gl_Position = MVP * vec4(a_pos, 1);
    v_normal = a_normal;
    v_texCoord = a_texCoords * 4.;
    v_y = a_y;
}