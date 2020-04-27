// #include "skinned_mesh.hpp"

// SkinnedMesh::SkinnedMesh(const aiMesh *mesh, const aiMaterial * mat, std::vector<Texture> textures, vector<VertexBoneData> bones, glm::mat4 transform)
// : Mesh(mesh, mat, textures, transform) {
//     this->bones = bones;
// }

// // render the mesh
// void SkinnedMesh::draw(Shader & shader) 
// {
//     glUniform1i(shader.uniform("has_bones"), true);
// }

// void SkinnedMesh::init(Shader & shader)
// {

//     GLint attribVertexBoneID  = shader.attribute("in_bone_ids");
//     GLint attribVertexBoneWeight  = shader.attribute("in_bone_weights");

//     glBindBuffer(GL_ARRAY_BUFFER, BBO);
//     glBufferData(GL_ARRAY_BUFFER, bones.size() * sizeof(VertexBoneData), &bones[0], GL_STATIC_DRAW);

//     // for (size_t i = 0; i < bones.size(); i++) {
//     //     std::cout << "Bone for: " << i << " has (" << bones[i].IDs[0] << ","<< bones[i].IDs[1] << ","<< bones[i].IDs[2] << ","<< bones[i].IDs[3] << ")." << std::endl;
//     //     std::cout << "Their weights are (" << bones[i].Weights[0] << ","<< bones[i].Weights[1] << ","<< bones[i].Weights[2] << ","<< bones[i].Weights[3] << ")." << std::endl;
//     // }

//     glEnableVertexAttribArray(attribVertexBoneID);
//     glVertexAttribIPointer(attribVertexBoneID, NUM_BONES_PER_VERTEX, GL_INT, sizeof(VertexBoneData), (void*)0);

//     glEnableVertexAttribArray(attribVertexBoneWeight);
//     glVertexAttribPointer(attribVertexBoneWeight, NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, Weights));
    
//     glBindVertexArray(0);
// }