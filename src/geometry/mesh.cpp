#include "mesh.hpp"

#include <glm/gtx/normal.hpp>

// Local Headers
#include "graphics/vert_buffer.hpp"

// Mesh::Mesh(std::string name, unsigned int nrOfVertices, unsigned int nrOfIndices, VertAttributes attributes)
//     : name(name), VertData(attributes, std::vector<float>(nrOfVertices * attributes.getVertSize()), std::vector<unsigned short>(nrOfIndices))
// {
//     std::cout << "Mesh created: " << name << std::endl;
//     vertBuffer = new VertBuffer(attributes, this);
// }

Mesh::Mesh(const std::string& name, unsigned int nrOfVertices, unsigned int nrOfIndices, VertAttributes attributes)

    : name(name), nrOfVertices(nrOfVertices), nrOfIndices(nrOfIndices),

      VertData(attributes, std::vector<u_char>(nrOfVertices * attributes.getVertSize())),
      indices(nrOfIndices)
{
    std::cout << "Mesh created: " << name << std::endl;
}


void Mesh::render() {
    if (!vertBuffer || !vertBuffer->isUploaded()) throw name + " is not uploaded. Upload it first with a VertBuffer";
    vertBuffer->bind();
    // draw mesh
    glDrawElements(
        GL_TRIANGLES,
        indices.size(),
        GL_UNSIGNED_SHORT,
        0
    );
}

void Mesh::computeSmoothingNormals() {
    
    std::vector<glm::vec3> smoothVertexNormals(nrOfVertices, glm::vec3(0.f));

    int position_offset = attributes.getOffset(VertAttributes::POSITION);
    int normal_offset = attributes.getOffset(VertAttributes::NORMAL);

    for (size_t f = 0; f < indices.size() / 3; f++) {
        int idx0 = indices[3 * f + 0];
        int idx1 = indices[3 * f + 1];
        int idx2 = indices[3 * f + 2];

        // Get the three indexes of the face (all faces are triangular)
        glm::vec3 v1 = get<glm::vec3>(idx0, position_offset);
        glm::vec3 v2 = get<glm::vec3>(idx1, position_offset);
        glm::vec3 v3 = get<glm::vec3>(idx2, position_offset);

        // Get the three vertex indexes and coordinates
        glm::vec3 normal = glm::triangleNormal(v1, v2, v3);

        // Add the normal to the three vertexes
        smoothVertexNormals[idx0] += normal; 
        smoothVertexNormals[idx1] += normal; 
        smoothVertexNormals[idx2] += normal; 

    }  // faces

    // Normalize the normals, that is, make them unit vectors
    for (size_t i = 0; i < smoothVertexNormals.size(); i++) {
        set(glm::normalize(smoothVertexNormals[i]), i, normal_offset); 
    }
}

Mesh::~Mesh() {
    if (vertBuffer)
        vertBuffer->onMeshDestroyed();
}