#version 330 core

out vec4 color;
in vec3 v_normal;

void main()
{
    color = vec4(v_normal, 1);
}
