#include "renderer.hpp"

#include "scene.hpp"

Renderer::Renderer() {}

void Renderer::draw_objects(const std::vector<Renderable *> & renderables, glm::mat4 parent_model, Shader & shader, RenderType type, bool update_model) {
    if (renderables.size() == 0) return;

    const Camera camera = Globals::scene->getCamera();
    
    for(auto t=renderables.begin(); t!=renderables.end(); ++t) {
        
        check_gl_error();
        // default_phong_uniforms();

        if (update_model) (*t)->update_model();

        glm::mat4 model = parent_model * (*t)->get_last_model();

        // glUniformMatrix4fv( shader.uniform("model"), 1, GL_FALSE, glm::value_ptr(model)); // model matrix
        glUniformMatrix4fv( shader.uniform("MVP"), 1, GL_FALSE, glm::value_ptr(camera.combined * model)); // projection matrix
        check_gl_error();
        (*t)->render(type);
        check_gl_error();

        // std::cout << "has children " << (*t)->get_children().size() << std::endl;
        draw_objects((*t)->get_children(), model, shader, type, update_model);

        // After all their children have been updated
        (*t)->update_bounding_box();
    }
}