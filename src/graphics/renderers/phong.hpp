#pragma once

// Standard Headers
#include <vector>
#include <glm/mat4x4.hpp>

// Local Headers
#include "graphics/shader.hpp"
#include "graphics/renderable.hpp"

struct Light {
    glm::vec4 position;
    glm::vec3 intensities;
    float attenuation; 
    float ambientCoefficient;

    Light(const glm::vec4 & position_, const glm::vec3 & intensities_, float attenuation_, float ambientCoefficient)
    : position(position_), intensities(intensities_), attenuation(attenuation_), ambientCoefficient(ambientCoefficient) {}
};

class PhongRenderer
{
    private:
        // Shader shader;
        std::vector<Light> gLights;
        void default_phong_uniforms();
        void render_objects(std::vector<Renderable *> renderables, glm::mat4 parent_model, Shader & shader);

    public:
        PhongRenderer();

        size_t add_model(const std::string &directory, const std::string &obj_file);
        void add_light(glm::vec4 pos, glm::vec3 strength, float attenuation, float ambient); 
        
        void upload(std::vector<Renderable *> objects);
        void draw(std::vector<Renderable *> objects);
};
