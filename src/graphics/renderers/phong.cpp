#include "phong.hpp"

// Standard Headers
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

// Local Headers
#include "scene.hpp"
#include "utils/resource_manager.hpp"

PhongRenderer::PhongRenderer(): gLights() {
    check_gl_error();
    Shader shader = ResourceManager::LoadShader("test.vert", "normaltest.frag", "phong");
    check_gl_error();

    add_light(
        glm::vec4(1.f, 3.f, 1.f, 1), //w == 1 indications a point light
        glm::vec3(0.9,0.9,0.9), //strong light
        5.0f, // Doesn't matter for direction
        0.5f // Little ambient
    );

    add_light(
        glm::vec4(1, 0.8, 0.6, 0), //w == 0 indications a directional light
        glm::vec3(0.2,0.2,0.2), //weak light
        0.0f, // Doesn't matter for direction
        0.9f // Little ambient
    );
    check_gl_error();
}

// void Phong::add_object(Renderable * entity, Renderable * parent) {
//     // entity->init(shader);

//     if (parent) {
//         parent->add_child(entity);
//     } else {
//         _objects.push_back(entity);
//     }
// }

void PhongRenderer::add_light(glm::vec4 pos, glm::vec3 strength, float attenuation, float ambient) {
    gLights.emplace_back(pos, strength, attenuation, ambient);
}

std::string CreateLightUniform(const char* propertyName, size_t lightIndex) {
    std::ostringstream ss;
    ss << "allLights[" << lightIndex << "]." << propertyName;
    return ss.str();
}

///////////////////////////////////////////////////////////////////////////////
// set uniform constants
///////////////////////////////////////////////////////////////////////////////
void PhongRenderer::default_phong_uniforms() {
    check_gl_error();

    Shader shader = ResourceManager::GetShader("phong");
    shader.enable();

    glUniform1i(shader.uniform("numLights"), (int)gLights.size());
    check_gl_error();

    float materialSpecular[] = {0.f, 0.f, 0.f};
    float materialShininess = 1;

    for(size_t i = 0; i < gLights.size(); ++i){
        glUniform4fv(shader.uniform(CreateLightUniform("position", i)), 1, glm::value_ptr(gLights[i].position));
        glUniform3fv(shader.uniform(CreateLightUniform("intensities", i)), 1, glm::value_ptr(gLights[i].intensities));
        glUniform1f(shader.uniform(CreateLightUniform("attenuation", i)), gLights[i].attenuation);
        glUniform1f(shader.uniform(CreateLightUniform("ambientCoefficient", i)), gLights[i].ambientCoefficient);
    }

    glUniform3fv(shader.uniform("materialSpecularColor"), 1, materialSpecular);
    glUniform1f(shader.uniform("materialShininess"), materialShininess);

    glUniform1i(shader.uniform("has_bones"), false);
    glUniform1i(shader.uniform("has_textures"), false);

    check_gl_error();
}

void PhongRenderer::upload(std::vector<Renderable *> objects) {
    Shader shader = ResourceManager::GetShader("phong");
    shader.enable();

    for(auto t=objects.begin(); t!=objects.end(); ++t) {
        (*t)->upload();
    }
}

void PhongRenderer::draw(std::vector<Renderable *> objects) {
    Shader shader = ResourceManager::GetShader("phong");
    shader.enable();

    
    // const Camera camera = Globals::scene->getCamera();
    // glUniform3fv(shader.uniform("eye"), 1, glm::value_ptr(camera.position));
    // glUniformMatrix4fv( shader.uniform("view"), 1, GL_FALSE, glm::value_ptr(camera.view) ); // viewing transformation
	// glUniformMatrix4fv( shader.uniform("projection"), 1, GL_FALSE, glm::value_ptr(camera.projection)); // projection matrix
    // glUniformMatrix4fv( shader.uniform("camera"), 1, GL_FALSE, glm::value_ptr(camera.combined)); // viewing transformation

    check_gl_error();
    render_objects(objects, glm::mat4(1.f), shader);

    shader.disable();
}

void PhongRenderer::render_objects(std::vector<Renderable *> renderables, glm::mat4 parent_model, Shader & shader) {
    if (renderables.size() == 0) return;

    const Camera camera = Globals::scene->getCamera();
    
    for(auto t=renderables.begin(); t!=renderables.end(); ++t) {
        
        check_gl_error();
        // default_phong_uniforms();

        (*t)->update_model();

        glm::mat4 model = parent_model * (*t)->get_last_model();

        // glUniformMatrix4fv( shader.uniform("model"), 1, GL_FALSE, glm::value_ptr(model)); // model matrix
        glUniformMatrix4fv( shader.uniform("MVP"), 1, GL_FALSE, glm::value_ptr(camera.combined * model)); // projection matrix
        check_gl_error();
        (*t)->render();
        check_gl_error();

        // std::cout << "has children " << (*t)->get_children().size() << std::endl;
        render_objects((*t)->get_children(), model, shader);

        // After all their children have been updated
        (*t)->update_bounding_box();
    }
}