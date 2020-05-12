#version 330 core
precision mediump float;

out vec4 color;

in vec3 v_texCoords;

uniform samplerCube cubemap;
uniform float atmosphere;

uniform float daytime;

void main()
{
    float sky = min(1., max(0., (atmosphere - .3) * 2.));
    
    color = vec4(.45, .55, .9, 1.) * sky * daytime + texture(cubemap, v_texCoords) * (1. - sky);
}
