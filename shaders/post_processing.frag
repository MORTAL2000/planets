#version 330 core
precision mediump float;

out vec4 color;

in vec2 v_texCoords;

uniform sampler2D scene;
uniform vec2 resolution;
uniform float zoomEffect;

uniform sampler2D sceneDepth;
uniform float zoom;

const float near = .1, far = 2000.;

void zoomMotionBlur()
{
    vec2 mbOffset = vec2(gl_FragCoord.x / resolution.x, gl_FragCoord.y / resolution.y) - vec2(.5);
    if (zoomEffect > .05)
    {
        const int steps = 20;
        float div = 1.;

        for (int i = 0; i < steps; i++)
        {
            float effect = 1. - float(i) / float(steps);
            div += effect;
            color += texture(scene, v_texCoords - mbOffset * .09 * zoomEffect * (float(i) / float(steps))) * effect;
        }
        color /= div;
    }
}

void fog()
{
    float depth = texture(sceneDepth, v_texCoords).r;
    depth = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    float fog = max(0., min(1., (depth * .24 - 63. * (1. - zoom + .1)) * .1));
    fog *= min(1., max(0., (zoom - .42) * 4.6));

    if (depth > 200. && color.r + color.g + color.b > 1.85) fog = 0.;

    color.rgb *= 1. - fog;
    color.rgb += vec3(.45, .55, .9) * fog;
}

void main()
{
    // float depthValue = texture(scene, v_texCoords).r;
    // color = vec4(vec3(depthValue), 1.0);

    vec2 offset = v_texCoords * vec2(-2) + vec2(1);
    // chromatic aberration
    float r = texture(scene, v_texCoords + offset * .0010).r;
    float g = texture(scene, v_texCoords + offset * .0005).g;
    float b = texture(scene, v_texCoords).b;

    color = vec4(r, g, b, 1);

    zoomMotionBlur();

    fog();

    float vignette = smoothstep(3.0, .6, length(offset));
    color.rgb *= vignette;
}
