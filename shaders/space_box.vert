#version 330 core
layout(location = 0) in vec3 a_pos;

out vec3 v_texCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    v_texCoords = a_pos;
    gl_Position = (projection * view * vec4(a_pos, 1.0)).xyww;
    // gl_Position = viewProjection * vec4(a_pos, 1.0);
}
