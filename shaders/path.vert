#version 330 core

layout(location = 0) in vec3 a_vertex;

uniform mat4 MVP;

out vec3 v_normal;

void main()
{
    gl_Position = MVP * vec4(a_vertex, 1);
}
