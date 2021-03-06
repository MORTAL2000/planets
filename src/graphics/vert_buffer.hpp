#pragma once

// System Headers
#include <glad/glad.h>

// Standard Headers
#include <memory>

// Local Headers
#include "vert_attributes.hpp"
#include "vert_data.hpp"
#include "geometry/mesh.hpp"

/**
 * A class that encapsulates OpenGL VertexArrayObjects, VertexBufferObjects, IndexBufferObjects and Instanced Arrays
 */
class VertBuffer
{
  public:

    // creates a VertBuffer for meshes with these attributes
    static VertBuffer* with(VertAttributes &attributes);

    // try not to use this. It is more efficient to put more meshes (with the same VertAttributes) in 1 VertBuffer
    static void uploadSingleMesh(SharedMesh mesh);

    // adds mesh to Meshes that are going to be uploaded when upload() is called.
    VertBuffer* add(SharedMesh mesh);

    /**
     * upload all added Meshes to OpenGL, after uploading the Meshes can be drawn.
     * 
     * if disposeOfflineData is true then mesh.disposeOfflineData() will be called for each uploaded mesh.
     **/
    void upload();

    bool isUploaded() const;

    void bind();

    void onMeshDestroyed(); // Called by ~Mesh()

    /**
     * upload vertex-attributes that do not advance per vertex, but per instance (glDrawElementsInstanced() & glVertexAttribDivisor())
     * also known as Instanced Arrays (https://www.khronos.org/opengl/wiki/Vertex_Specification#Instanced_arrays)
     *
     * useful for when you want to render a mesh multiple times in 1 draw call, but each on a different position.
     **/
    GLuint uploadPerInstanceData(VertData data, GLuint advanceRate=1);

    void usePerInstanceData(GLuint instanceDataId, GLuint advanceRate=1);

    void deletePerInstanceData(GLuint instanceDataId);

    ~VertBuffer();

  private:

    // in WebGL there's a limit for the amount of vertices in a VertBuffer, when exceeding this amount the remaining meshes will be uploaded to 'next'
    // VertBuffer *next = NULL;

    VertBuffer(VertAttributes &attributes);

    // returns wether the stored vertex data is actually used by Meshes
    bool inUse() const;

    void setAttrPointersAndEnable(VertAttributes &attributes, unsigned int divisor=0, unsigned int locationOffset=0);

    // ids of the VertexArrayObject, VertexBufferObject and IndexBufferObject
    GLuint vaoId = 0, vboId = 0, iboId;

    std::vector<GLuint> instanceVbos;
    std::vector<VertAttributes> instanceVboAttrs;
    
    GLuint nrOfVerts = 0, vertSize = 0, nrOfIndices = 0;

    std::vector<std::weak_ptr<Mesh>> meshes;

    VertAttributes attrs;

    bool uploaded = false;

    static GLuint currentlyBoundVao;
};
