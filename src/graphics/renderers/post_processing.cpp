#include "post_processing.hpp"

#include "scene.hpp"
#include "graphics/window_size.hpp"
// #include "graphics/camera.hpp"
// #include "common/universe.hpp"
#include "utils/resource_manager.hpp"

PostProcessing::PostProcessing()  {
    Shader shader = ResourceManager::LoadShader("post_processing.vert", "post_processing.frag", "post_processing");
    Shader flare = ResourceManager::LoadShader("post_processing.vert", "flare.frag", "flare");

    flareTextures = ResourceManager::LoadTextureArray({
        "textures/sun/flare1.dds",
        "textures/sun/flare0.dds",
        "textures/sun/flare3.dds",
        "textures/sun/flare4.dds",
        "textures/sun/flare5.dds"
    }, "flares");


    check_gl_error();
}


const LensFlare PostProcessing::flares[] = {
    {4, vec4(1), 1, 1, false},

    {1, vec4(1, 1, 1, .35), 1, -.8, true},

    {3, vec4(.7, .8, .9, .05), .25, -1.6, false},
    {3, vec4(.9, .8, .4, .1), .38, -1.5, false},
    {3, vec4(.7, .9, .8, .15), .45, -1.4, false},

    {3, vec4(.9, .9, .7, .1), .18, -.75, false},

    {0, vec4(.9, .9, .7, .7), .05, -.74, false},
    {0, vec4(1), .015, -.75, false},

    {0, vec4(.2, .3, 2, .7), .05, -.8, false},
    {0, vec4(1), .015, -.81, false},

    {3, vec4(.9, .9, .7, .07), .5, -.5, false},
    {3, vec4(.9, .9, .7, .03), .8, -.54, false},

    {3, vec4(.1, 1, .2, .2), .1, -.5, false},
    {3, vec4(.1, 1, .2, .2), .09, -.47, false},
    {0, vec4(.7, 1, .7, 1), .008, -.48, false},

    {0, vec4(.1, .7, 1, .4), .06, .1, false},
    {0, vec4(.7, 1, .7, 1), .008, .11, false},

    {3, vec4(.8, .8, .3, .2), .06, .23, false},
    {0, vec4(1, 1, .7, 1), .008, .225, false},

    {0, vec4(.6, 1, .8, 1), .1, .23, false},

    {2, vec4(1, .3, .1, .2), .5, 1.03, false},
    {3, vec4(.6, .4, 1, .15), .4, 1.55, false},

    {0, vec4(.2, 1, .5, .3), .03, 1.8, false},
    {0, vec4(.2, 1, .5, 1), .008, 1.81, false},
};

void PostProcessing::render() {

    Shader shader = ResourceManager::GetShader("post_processing");
    shader.enable();

    glDisable(GL_DEPTH_TEST);

    applyUniforms(shader);
    Mesh::getQuad()->render();

    Shader flareShader = ResourceManager::GetShader("flare");
    flareShader.enable();

    glBlendFunc(GL_ONE, GL_ONE);

    renderFlares(flareShader);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void PostProcessing::renderFlares(Shader & shader) {

    float lensFlareA = 1;

    int steps = 50;

    const Universe & universe = Globals::scene->getUniverse();
    const Camera & camera = Globals::scene->getCamera();
    const glm::vec3 & cameraPosition = camera.getPosition();
    
    glm::vec2 x(0.f);
    for (int i = 0; i < steps; i++)
    {
        vec3 sd = glm::rotate(camera.sunDir, (float) (.05 * (((float) i - steps * .5) / steps)), mu::Y);
        vec3 sunPos = cameraPosition + sd;

        bool inViewport = false;
        camera.project(sunPos, inViewport);
        bool intersectsPlanet = false;

        for(Planet * planet : universe.getPlanets()) {
            if (planet->rayToLonLat(cameraPosition, sd, x)) {
                intersectsPlanet  = true;
                break;
            }
        }

        if (!inViewport || intersectsPlanet)
            lensFlareA -= 1.0 / steps;
    }

    lensFlareA = pow(lensFlareA, 2.);
    lensFlareA = lensFlareAlpha = (lensFlareA + lensFlareAlpha) * .5;
    
    vec2 screenSunPos = camera.project(cameraPosition + camera.sunDir);

    lensFlareA *= 1.4 - length(screenSunPos);
    lensFlareA *= .45;

    flareTextures->bind(0);
    glUniform1i(shader.uniform("textures"), 0);

    glm::mat4 correctScale = scale(mat4(1), vec3(((float) WindowSize::height / WindowSize::width), 1, 1));

    for (auto &flare : flares)
    {
        mat4 mvp = translate(scale(mat4(1), vec3(flare.scale)), vec3(screenSunPos * flare.dist / flare.scale, 1)) * correctScale;

        if (flare.rotate)
        {
            mvp = glm::rotate(
                mvp,
                (float) -atan2(screenSunPos.x, screenSunPos.y) + mu::PI * -1.25f,
                mu::Z
            );
        }
	    
        glUniformMatrix4fv(shader.uniform("MVP"), 1, GL_FALSE, &(mvp)[0][0]);
        glUniform1i(shader.uniform("layer"), flare.texture);
        glUniform4f(shader.uniform("flareColor"), flare.color.r, flare.color.g, flare.color.b, flare.color.a * lensFlareA);

        Mesh::getQuad()->render();
    }
    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void PostProcessing::applyUniforms(Shader & shader) {
    
    glUniformMatrix4fv(shader.uniform("MVP"), 1, GL_FALSE, &(glm::mat4(1.f))[0][0]);
    glUniform1f(shader.uniform("zoomEffect"), Globals::scene->planetCamera.zoomVelocity / 2.);
    // glUniform1f(shader.uniform("zoom"), Globals::scene->planetCamera.atmosphereTilt);
    glUniform2f(shader.uniform("resolution"), WindowSize::widthPixels, WindowSize::heightPixels);

    // Bind results from other buffers
    Globals::scene->sceneBuffer->colorTexture->bind(0);
    glUniform1i(shader.uniform("scene"), 0);

    // Globals::scene->sceneBuffer->depthTexture->bind(1);
    // glUniform1i(shader.uniform("sceneDepth"), 1);
}
