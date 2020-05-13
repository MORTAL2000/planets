#version 330 core
precision mediump float;

out vec4 color;

in vec3 v_color;
in float v_alpha;
in vec3 v_normal;

void main()
{
    color = vec4(v_color, v_alpha);
}
