#version 330 core
precision mediump float;

in vec2 v_texCoord;

out vec4 color;

uniform sampler2D sunTexture;

void main() {
    color = texture(sunTexture, v_texCoord);
}