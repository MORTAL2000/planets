#version 330 core
precision mediump float;

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;

uniform mat4 MVP;
uniform mat4 atmoModel;

uniform vec3 sunDir;
uniform vec3 camPos;

out vec3 v_color;
out float v_alpha;

float clamp1(float x)
{
    return min(1., max(0., x));
}

void main()
{
    gl_Position = MVP * vec4(a_pos, 1);

    // v_alpha = 0.1;
    // v_color = vec3(.3, .5, 1);

    // camDist;
    // camPos;
    sunDir;

    float highlight = pow(clamp1(dot(a_normal, sunDir) + 0.5), 2) + 0.1;
    float shadow = clamp1(dot(a_normal, sunDir));
    
    // float camDot;
    // float edge = 1.0;
    // float middle = camDot * 9. * max(.1, clamp1(1. - camDist / 500.));
    // middle = clamp1(middle);
    // v_alpha = middle - edge;

    // float sunDot = dot(a_normal, sunDir);

    // v_alpha *= clamp1(sunDot * 2.) + .5;
    // v_alpha += .5;
    v_alpha = 0.2;
    v_color = vec3(.3, .5, 1) * highlight;
}
