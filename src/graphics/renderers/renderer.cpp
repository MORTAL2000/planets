#include "renderer.hpp"

#include "scene.hpp"

Renderer::Renderer(): camera(&Globals::scene->camera) {}
Renderer::Renderer(Camera * camera): camera(camera) {}

void Renderer::draw_objects(const std::vector<Renderable *> & renderables, glm::mat4 parent_model, Shader & shader, RenderType type, bool update_model) {
    if (renderables.size() == 0) return;

    if (!camera) throw "No Camera"; 

    for(auto t=renderables.begin(); t!=renderables.end(); ++t) {
        
        check_gl_error();
        // default_phong_uniforms();

        if (update_model) (*t)->update_model();

        glm::mat4 model = parent_model * (*t)->get_last_model();

        // glUniformMatrix4fv( shader.uniform("model"), 1, GL_FALSE, glm::value_ptr(model)); // model matrix
        glUniformMatrix4fv( shader.uniform("MVP"), 1, GL_FALSE, glm::value_ptr(camera->combined * model)); // projection matrix
        
        if (shadows) {
            mat4 shadowMatrix = ShadowRenderer::BIAS_MATRIX * Globals::scene->shadow_renderer->camera->combined * model; 
            glUniformMatrix4fv(shader.uniform("shadowMatrix"), 1, GL_FALSE, &((shadowMatrix)[0][0]));
        }

        check_gl_error();
        (*t)->render(type);
        check_gl_error();

        // std::cout << "has children " << (*t)->get_children().size() << std::endl;
        draw_objects((*t)->get_children(), model, shader, type, update_model);

        // After all their children have been updated
        (*t)->update_bounding_box();
    }
}
