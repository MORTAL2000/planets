#version 330 core
precision mediump float;

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in vec3 a_tangent;

out vec2 v_texCoord;

uniform mat4 MVP;

void main(void) {
    gl_Position = MVP * vec4(a_pos, 1.0);
    v_texCoord = a_texCoords;
}