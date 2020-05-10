#include "shadow_renderer.hpp"

#include <iostream>

ShadowRenderer::ShadowRenderer()
    : sunCam(150, 150, 90), buffer(2048, 2048)
{
    buffer.addDepthTexture(GL_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    sunDepthTexture = buffer.depthTexture;
}

void ShadowRenderer::begin(const Camera &mainCam, const vec3 &sunDir)
{
    vec3 frustumCenter = mainCam.position + mainCam.direction * float(length(mainCam.position) - 130);

    sunCam.position = frustumCenter - sunDir * float(120);
    sunCam.lookAt(frustumCenter);

    sunCam.calculate(0, 300);

    buffer.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowRenderer::end()
{
    buffer.unbind();
}
