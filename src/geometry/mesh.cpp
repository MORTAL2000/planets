#include "mesh.hpp"

#include <glm/gtx/normal.hpp>

// Local Headers
#include "graphics/vert_buffer.hpp"

SharedMesh Mesh::quad;

SharedMesh Mesh::getQuad()
{
    if (!quad)
    {
        quad = SharedMesh(new Mesh("quad", 4, 6, VertAttributes().add_(VertAttributes::POSITION)));
        quad->set<float[12]>({
                -1, -1, 0,
                -1, 1, 0,
                1, 1, 0,
                1, -1, 0,
            }, 0, 0);
        quad->indices.insert(quad->indices.begin(), {
            2, 1, 0,
            0, 3, 2,
        });
        VertBuffer::uploadSingleMesh(quad);
    }
    return quad;
}


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
    check_gl_error();
    // draw mesh
    glDrawElementsBaseVertex(
        GL_TRIANGLES,
        nrOfIndices,
        GL_UNSIGNED_SHORT,
        (void *)(uintptr_t)indicesBufferOffset,
        baseVertex
    );
    check_gl_error();
}


void Mesh::renderInstances(GLsizei count)
{
    if (!vertBuffer || !vertBuffer->isUploaded()) throw name + " is not uploaded. Upload it first with a VertBuffer";
    vertBuffer->bind();
   
    glDrawElementsInstancedBaseVertex(
        GL_TRIANGLES,
        nrOfIndices,
        GL_UNSIGNED_SHORT,
        (void *)(uintptr_t)indicesBufferOffset,
        count,
        baseVertex
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
    std::cout << "Mesh destroyed: " << name << std::endl;
    
    if (vertBuffer)
        vertBuffer->onMeshDestroyed();
}