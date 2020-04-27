#include "terrain_face.hpp"

#include <glm/glm.hpp>

TerrainFace::TerrainFace(glm::vec3 localUp, int resolution):
    localUp(localUp), 
    resolution(resolution) {
    axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
    axisB = glm::cross(localUp, axisA);
}

void TerrainFace::createMesh()
{
    std::string name = "terrain_face";
    VertAttributes attrs;

    unsigned int posOffset = attrs.add(VertAttributes::POSITION);
    attrs.add(VertAttributes::NORMAL);
        // uvOffset = attrs.add(VertAttributes::TEX_COORDS),
        // tanOffset = attrs.add(VertAttributes::TANGENT),
        // texOffset = attrs.add({"TEX_BLEND", 4, GL_FALSE}),
        // yLevelOffset = attrs.add({"Y_LEVEL", 1, GL_FALSE});

    int numVerts = resolution * resolution;
    int numTriangles = (resolution - 1) * (resolution - 1) * 6;

    mesh = SharedMesh(new Mesh(name + "_mesh", numVerts, numTriangles, attrs));

    // for (int i = 0; i < numVerts; i++)
    // {
    //     mesh->set(vertexNormalsPlanet[i], i, norOffset);       // normal

    //     mesh->set(
    //             glm::vec2(vertIToX(i), vertIToY(i)) / vec2(50), i, uvOffset    // texCoords
    //     );

    //     mesh->set(textureMap[i], i, texOffset);    // texture blending
    //     mesh->setFloat(vertexPositionsOriginal[i].y, i, yLevelOffset); // y level
    // }

    int index = 0;
    for (int y = 0; y < resolution; y++)
    {
        for (int x = 0; x < resolution; x++)
        {
            int i = x + y * resolution;
            glm::vec2 percent = glm::vec2(x, y) / (resolution - 1.f);
            glm::vec3 pointOnUnitCube = localUp + (percent.x - .5f) * 2 * axisA + (percent.y - .5f) * 2 * axisB;
            glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);

            mesh->set(pointOnUnitSphere, i, posOffset);     // position
            
            if (x != resolution - 1 && y != resolution - 1)
            {
                mesh->indices[index] = i;
                mesh->indices[index + 1] = i + resolution + 1;
                mesh->indices[index + 2] = i + resolution;

                mesh->indices[index + 3] = i;
                mesh->indices[index + 4] = i + 1;
                mesh->indices[index + 5] = i + resolution + 1;
                index += 6;
            }
        }
    }

    // Calculate normals
        mesh->computeSmoothingNormals();
    // Calculate tangents
    // Calculate UV
}
