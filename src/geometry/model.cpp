#include "model.hpp"

// Standard Headers
#include <glm/gtc/type_ptr.hpp>

// Local Headers
#include "scene.hpp"
#include "utils/stb_image.h"
#include "utils/resource_manager.hpp"

// https://github.com/capnramses/antons_opengl_tutorials_book/blob/master/30_skinning_part_one/main.cpp
/*  Functions   */
// constructor, expects a filepath to a 3D model.
Model::Model(std::string const &path, const glm::vec3 & scale, bool gamma = false) : gamma_correction(gamma)
{
    // m_NumBones = 0;
    global_transform = glm::scale(glm::mat4(1.f), scale);

    global_transform_inverse = glm::inverse(global_transform);

    // set_scale(scale);
    load_model(path);
}

void Model::load_model(std::string const &path) {
    // read file via ASSIMP
    const aiScene* scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Fast 
                                    // aiProcess_LimitBoneWeights      |
                                    // aiProcess_FlipUVs               |          
                                    // aiProcess_GenSmoothNormals      |
                                    // aiProcess_OptimizeMeshes        |
                                    // aiProcess_OptimizeGraph         |
                                    // aiProcess_FixInfacingNormals    |
                                    // aiProcess_FindInvalidData       |
                                    // aiProcess_CalcTangentSpace      |
                                    // aiProcess_SortByPType           
                                    // aiProcess_PreTransformVertices
    );
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        throw 0;
    }
 
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    m_GlobalInverseTransform = glm::inverse(aiMatrix4x4ToGlm(&scene->mRootNode->mTransformation));

//    if (scene->HasAnimations()) {
//        std::cout << "Scene has " << scene->mNumAnimations << " animations!" << std::endl;
//
//        for (size_t i = 0; i < scene->mNumAnimations; i++) {
//            processAnimation(scene->mAnimations[i], scene);
//        }
//    }

    if (scene->HasLights()) {
        std::cout << "Scene has " << scene->mNumLights << " lights!" << std::endl;

        // for(unsigned int i = 0; i < scene->mNumLights; i++) {
        //     aiLight * light = scene->mLights[i];
        //     Globals::scene->add_light(
        //         glm::vec4(light->mPosition.x,light->mPosition.y,light->mPosition.z, 1.0), 
        //         glm::vec3(light->mPosition.x,light->mPosition.y,light->mPosition.z), light->mAttenuationLinear, 0.05f);
        // }
    }

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

// inits the model
void Model::upload() {}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode *node, const aiScene* scene)
{
    // Renderable * node_placeholder = new DummyRenderable();
    // node_placeholder->set_parent_matrix(aiMatrix4x4ToGlm(&node->mTransformation));

    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        // Globals::scene->add_renderable(processMesh(scene->mMeshes[node->mMeshes[i]]), this);
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

    // Globals::scene->add_renderable(node_placeholder, parent);
}

SharedMesh Model::processMesh(aiMesh * mesh, const aiScene* scene) {
    // data to fill
    // vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    VertAttributes att;

    if (mesh->HasPositions()) att.add(VertAttributes::POSITION);
    if (mesh->HasNormals()) att.add(VertAttributes::NORMAL);
    if (mesh->HasTextureCoords(0)) att.add(VertAttributes::TEX_COORDS);
    if (mesh->HasVertexColors(0)) att.add(VertAttributes::RGBA);
    if (mesh->HasTangentsAndBitangents()) {
        att.add(VertAttributes::TANGENT);
        att.add(VertAttributes::BI_NORMAL);
    }

    std::vector<Texture> textures;

    SharedMesh sharedmesh = SharedMesh(new Mesh(mesh->mName.C_Str(), mesh->mNumVertices, (unsigned int) indices.size(), att));

    // Walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        if (mesh->HasPositions()) {
            glm::vec3 position( mesh->mVertices[i].x,  mesh->mVertices[i].y,  mesh->mVertices[i].z); 
            sharedmesh->set(position, i, att.getOffset(VertAttributes::POSITION));
        }

        if (mesh->HasNormals()) {
            glm::vec3 position( mesh->mNormals[i].x,  mesh->mNormals[i].y,  mesh->mNormals[i].z); 
            sharedmesh->set(position, i, att.getOffset(VertAttributes::NORMAL));
        }

        if (mesh->HasVertexColors(0)) {
            glm::vec4 color( mesh->mColors[0][i].r,  mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a); 
            sharedmesh->set(color, i, att.getOffset(VertAttributes::RGBA));
        }

        if (mesh->HasTextureCoords(0)) {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            glm::vec2 textcord(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            sharedmesh->set(textcord, i, att.getOffset(VertAttributes::TEX_COORDS));
        }

        if (mesh->HasTangentsAndBitangents()) {
            glm::vec3 tangets( mesh->mTangents[i].x,  mesh->mTangents[i].y,  mesh->mTangents[i].z); 
            sharedmesh->set(tangets, i, att.getOffset(VertAttributes::TANGENT));

            glm::vec3 binormals( mesh->mBitangents[i].x,  mesh->mBitangents[i].y,  mesh->mBitangents[i].z); 
            sharedmesh->set(binormals, i, att.getOffset(VertAttributes::BI_NORMAL));
        }
    }

    // std::cout << "Loaded " << name.C_Str() << std::endl;
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];  
    Material & meshMaterial = sharedmesh->material;

    // Material ambient color
    aiColor3D ambient(0.f,0.f,0.f);
    material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    meshMaterial.ambient[0] = ambient.r;
    meshMaterial.ambient[1] = ambient.g;
    meshMaterial.ambient[2] = ambient.b;

    aiColor3D diffuse(0.f,0.f,0.f);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    meshMaterial.diffuse[0] = diffuse.r;
    meshMaterial.diffuse[1] = diffuse.g;
    meshMaterial.diffuse[2] = diffuse.b;

    // Material specular color
    aiColor3D specular(0.f,0.f,0.f);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    meshMaterial.specular[0] = specular.r;
    meshMaterial.specular[1] = specular.g;
    meshMaterial.specular[2] = specular.b;

    aiColor3D emission(0.f,0.f,0.f);
    material->Get(AI_MATKEY_COLOR_EMISSIVE, emission);
    meshMaterial.emission[0] = emission.r;
    meshMaterial.emission[1] = emission.g;
    meshMaterial.emission[2] = emission.b;

    meshMaterial.shininess = 1;
    material->Get(AI_MATKEY_SHININESS, meshMaterial.shininess);

    loadMaterialTextures(sharedmesh, material, aiTextureType_DIFFUSE, scene, "texture_diffuse");
    loadMaterialTextures(sharedmesh, material, aiTextureType_SPECULAR, scene, "texture_specular");
    loadMaterialTextures(sharedmesh, material, aiTextureType_HEIGHT, scene, "texture_normal");
    loadMaterialTextures(sharedmesh, material, aiTextureType_AMBIENT, scene, "texture_height");

    if (mesh->HasBones()) {
        std::cout << "Model has bones" << std::endl;
        // Upgrade to a skinnedmesh
        // return new SkinnedMesh(mesh, material, textures, loadBones(mesh), global_transform);
    }

    return sharedmesh;
}

//void Model::processAnimation(aiAnimation * aianimation, const aiScene* scene) {
//    Animation animation;
//
//    animation.ticks_per_second = (float)(aianimation->mTicksPerSecond != 0 ? aianimation->mTicksPerSecond : 25.0f);
//    animation.duration = aianimation->mDuration;
//}


// std::vector<VertexBoneData> Model::loadBones(const aiMesh* mesh) {
//     std::cout << "Loading bones" << std::endl;

//     vector<VertexBoneData> bones;
//     bones.resize(mesh->mNumVertices);

//     for (uint i = 0 ; i < mesh->mNumBones ; i++) {                
//         uint BoneIndex = 0;        
//         string BoneName(mesh->mBones[i]->mName.data);
        
//         if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
//             // Allocate an index for a new bone
//             BoneIndex = m_NumBones;
//             m_NumBones++;            
// 	        BoneInfo bi;			
// 			m_BoneInfo.push_back(bi);
//             m_BoneInfo[BoneIndex].BoneOffset = aiMatrix4x4ToGlm(&mesh->mBones[i]->mOffsetMatrix);            
//             m_BoneMapping[BoneName] = BoneIndex;
//         }
//         else {
//             BoneIndex = m_BoneMapping[BoneName];
//         }
                    
        
//         for (uint j = 0 ; j < mesh->mBones[i]->mNumWeights ; j++) {
//             uint VertexID = mesh->mBones[i]->mWeights[j].mVertexId;
//             float Weight  = mesh->mBones[i]->mWeights[j].mWeight;

//             bones[VertexID].AddBoneData(BoneIndex, Weight);
//         }
//     }  

//     return bones; 
// }

// std::string CreateBoneUniform(size_t boneIndex) {
//     std::ostringstream ss;
//     ss << "bones[" << boneIndex << "]";
//     return ss.str();
// }

//void Model::update_animation(float time) {
//    if (current_animation == -1) return;
//
////    Animation animation = animations[current_animation];
////
////    float time_in_ticks = time * animation.ticks_per_second;
////    float animation_time = fmod(time_in_ticks, animation.duration);
//
//    // std::cout << "ticks_per_second: " << ticks_per_second << std::endl;
//    // std::cout << "time_in_ticks: " << time_in_ticks << std::endl;
//    // std::cout << "animation_time: " << animation_time << " out of " << (float)scene->mAnimations[0]->mDuration << std::endl;
//    // calculate_animation(num, animation_time, scene->mRootNode, glm::mat4(1.f));
//}

uint Model::find_position(float time, const aiNodeAnim* pNodeAnim)
{    
    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (time < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }

    std::cout << "Unable to find next position keyframe" << std::endl;

    return 0;
}


uint Model::find_rotation(float time, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        if (time < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }
    
    std::cout << "Unable to find next rotation keyframe" << std::endl;

    return 0;
}


uint Model::find_scaling(float time, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);
    
    for (uint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        
        if (time < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }

    std::cout << "Unable to find next scaling keyframe" << std::endl;

    return 0;
}

glm::quat Model::calculate_rotation(float time, const aiNodeAnim* pNodeAnim) {
    if (pNodeAnim->mNumRotationKeys == 1) {
        return aiQuaternionToGlm(&pNodeAnim->mRotationKeys[0].mValue);
    }

    uint keyframe = find_rotation(time, pNodeAnim);
    uint next_keyframe = keyframe + 1;

    assert(next_keyframe < pNodeAnim->mNumRotationKeys);
    float dt = (float)(pNodeAnim->mRotationKeys[next_keyframe].mTime - pNodeAnim->mRotationKeys[keyframe].mTime);

    float factor = (time - (float)pNodeAnim->mRotationKeys[keyframe].mTime) / dt;

    glm::quat start = aiQuaternionToGlm(&pNodeAnim->mRotationKeys[keyframe].mValue);
    if(factor >= 0.0f && factor <= 1.0f) {
        glm::quat end = aiQuaternionToGlm(&pNodeAnim->mRotationKeys[next_keyframe].mValue);
        return glm::mix(start, end, factor);
    } else {
        return start;
    }
}

glm::vec3 Model::calculate_position(float time, const aiNodeAnim* pNodeAnim) {
    if (pNodeAnim->mNumPositionKeys == 1) {
        return aiVector3DToGlm(&pNodeAnim->mPositionKeys[0].mValue);
    }

    uint keyframe = find_position(time, pNodeAnim);
    uint next_keyframe = keyframe + 1;

    assert(next_keyframe < pNodeAnim->mNumPositionKeys);
    float dt = (float)(pNodeAnim->mPositionKeys[next_keyframe].mTime - pNodeAnim->mPositionKeys[keyframe].mTime);

    float factor = (time - (float)pNodeAnim->mPositionKeys[keyframe].mTime) / dt;

    glm::vec3 start = aiVector3DToGlm(&pNodeAnim->mPositionKeys[keyframe].mValue);

    if(factor >= 0.0f && factor <= 1.0f) {
        glm::vec3 end = aiVector3DToGlm(&pNodeAnim->mPositionKeys[next_keyframe].mValue);
        return glm::mix(start, end, factor);
    } else {
        return start;
    }
}

glm::vec3 Model::calculate_scaling(float time, const aiNodeAnim* pNodeAnim) {
    if (pNodeAnim->mNumScalingKeys == 1) {
        return aiVector3DToGlm(&pNodeAnim->mScalingKeys[0].mValue);
    }

    uint keyframe = find_scaling(time, pNodeAnim);
    uint next_keyframe = keyframe + 1;

    assert(next_keyframe < pNodeAnim->mNumScalingKeys);
    float dt = (float)(pNodeAnim->mScalingKeys[next_keyframe].mTime - pNodeAnim->mScalingKeys[keyframe].mTime);

    float factor = (time - (float)pNodeAnim->mScalingKeys[keyframe].mTime) / dt;
    glm::vec3 start = aiVector3DToGlm(&pNodeAnim->mScalingKeys[keyframe].mValue);

    if(factor >= 0.0f && factor <= 1.0f) {    
        glm::vec3 end = aiVector3DToGlm(&pNodeAnim->mScalingKeys[next_keyframe].mValue);
        return glm::mix(start, end, factor);
    } else {
        return start;
    }
}

// void Model::calculate_animation(int num, float time, const aiNode* pNode, const glm::mat4 & parent) {
//     std::string NodeName(pNode->mName.data);

//     const aiAnimation* pAnimation = scene->mAnimations[num];
//     glm::mat4 node_transformation = aiMatrix4x4ToGlm(&pNode->mTransformation);
    
//     const aiNodeAnim* pNodeAnim = find_node_animation(pAnimation, NodeName);

//     if (pNodeAnim) {
//         node_transformation = glm::mat4(1.f);

//         if (pNodeAnim->mNumScalingKeys > 0) {
//             glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), calculate_scaling(time, pNodeAnim));
//             node_transformation *= scale_matrix;
//         }

//         // Interpolate rotation and generate rotation transformation matrix
//         if (pNodeAnim->mNumPositionKeys > 0) {
//             glm::mat4 rotation_matrix = glm::toMat4(calculate_rotation(time, pNodeAnim));
//             node_transformation *= rotation_matrix;
//         }

//         // Interpolate translation and generate translation transformation matrix
//         if (pNodeAnim->mNumPositionKeys > 0) {
//             glm::mat4 translate_matrix = glm::translate( glm::mat4(1.0f), calculate_position(time, pNodeAnim));
//             node_transformation *= translate_matrix;
//         }
//     }

//     glm::mat4 global_transformation = parent * node_transformation;

//     // if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
//     //     uint BoneIndex = m_BoneMapping[NodeName];
//     //      m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * global_transform * global_transformation * m_BoneInfo[BoneIndex].BoneOffset * global_transform_inverse;
//     // }

//     for (uint i = 0 ; i < pNode->mNumChildren ; i++) {
//         calculate_animation(num, time, pNode->mChildren[i], global_transformation);
//     }
// }

const aiNodeAnim* Model::find_node_animation(const aiAnimation* pAnimation, const std::string NodeName)
{
    for (uint i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
        
        if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }
    
    return NULL;
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
void Model::loadMaterialTextures(SharedMesh mesh, aiMaterial *mat, aiTextureType type, const aiScene * scene, std::string typeName)
{
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str; 
        mat->GetTexture(type, i, &str);
        std::string path = str.C_Str();
        if (!ResourceManager::HasTexture(path))
        {
            if (const aiTexture * texture_ai = scene->GetEmbeddedTexture(str.C_Str())) {
                unsigned int nPixels = texture_ai->mWidth * (texture_ai->mHeight != 0 ? texture_ai->mHeight : 1); 
                ResourceManager::LoadTextureFromMemory(reinterpret_cast<unsigned char*>(texture_ai->pcData), nPixels, path);
            } else {
                ResourceManager::LoadTexture(directory + '/' + path, path);
            }
        }

        mesh->textures.push_back(path);
    }
}

// draws the model, and thus all its meshes
void Model::render() {

    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i]->render();

    // glUniformMatrix4fv( shader.uniform("bones"), bones.size(), GL_FALSE, glm::value_ptr(bones[0]));

    // for (size_t i= 0; i < m_NumBones; i++) {
        // std::cout << "Bone for: " << i << " has (" << glm::to_string(m_BoneInfo[i].FinalTransformation) << ")." << std::endl;
        // glUniformMatrix4fv( shader.uniform(CreateBoneUniform(i)), 1, GL_FALSE, glm::value_ptr(m_BoneInfo[i].FinalTransformation)  ); // model transformation
    // }
}
//
//GLuint TextureFromData(const GLvoid *data, int width, int height, int nrComponents) {
//    check_gl_error();
//
//    GLuint textureID;
//    glGenTextures(1, &textureID);
//    
//    GLenum format;
//    if (nrComponents == 1)
//        format = GL_RED;
//    else if (nrComponents == 3)
//        format = GL_RGB;
//    else if (nrComponents == 4)
//        format = GL_RGBA;
//
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//    check_gl_error();
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    check_gl_error();
//
//
//    return textureID;
//}
