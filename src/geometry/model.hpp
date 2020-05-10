#pragma once

// Standard Headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/normal.hpp>

// Local Headers
#include "mesh.hpp"
// #include "skinned_mesh.hpp"
#include "graphics/gl_error.hpp"

class Model;
typedef std::shared_ptr<Model> SharedModel;

struct Animation {
    std::string name;
    double ticks_per_second;
    double duration;
};

class Model
{
public:
    // std::map<string,uint> m_BoneMapping;
    // uint m_NumBones;
    // std::vector<BoneInfo> m_BoneInfo;

    std::string name;

    std::vector<SharedMesh> meshes;
    std::vector<Animation> animations;
    std::string directory;
    

    glm::mat4 global_transform;
    glm::mat4 global_transform_inverse;

    bool gamma_correction;

    /*  Functions   */
    // constructor, expects a filepath to a 3D model.
    Model(std::string const &path, const glm::vec3 & scale, bool gamma);

    // draws the model, and thus all its meshes
    void upload();
    void render();

    // int current_animation = -1;
    // void set_animation(int num) {
    //     if (num >= (int) animations.size()) {
    //         throw "Asking for animation that doesn't exist";
    //     }

    //     current_animation = num;
    // }

    // void update_animation(float time);

private:
    Assimp::Importer importer;
    glm::mat4 m_GlobalInverseTransform;


    // Based off of https://frame.42yeah.casa/2019/12/10/model-loading.html
    void load_model(std::string const &path);
    void processNode(aiNode *node, const aiScene* scene);
    SharedMesh processMesh(aiMesh *mesh, const aiScene* scene);
    // void material_uniforms(Shader & shader, const aiMaterial material);
    void loadMaterialTextures(SharedMesh mesh, aiMaterial *mat, aiTextureType type, const aiScene * scene, std::string typeName);
    // vector<VertexBoneData> loadBones(const aiMesh* mesh);

    // void processAnimation(aiAnimation * animation, const aiScene* scene);

    uint find_scaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint find_rotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint find_position(float AnimationTime, const aiNodeAnim* pNodeAnim);

    glm::vec3 calculate_scaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    glm::quat calculate_rotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    glm::vec3 calculate_position(float AnimationTime, const aiNodeAnim* pNodeAnim);

    const aiNodeAnim* find_node_animation(const aiAnimation* pAnimation, const std::string NodeName);
    // void calculate_animation(int num, float time, const aiNode* pNode, const glm::mat4 & parent);

    inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from)
    {
        glm::mat4 to;

        to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
        to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
        to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
        to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;

        return to;
    }
    inline glm::vec3 aiVector3DToGlm(const aiVector3D* from)
    {
        return glm::vec3(from->x, from->y, from->z);
    }
    inline glm::quat aiQuaternionToGlm(const aiQuaternion* from)
    {
        return glm::quat(from->w, from->x, from->y, from->z);
    }
};
