#include "sphere.hpp"

// Standard Headers
#include <glm/glm.hpp>

// Local Headers
#include "graphics/vert_attributes.hpp"
#include "graphics/tangent_calculator.hpp"
#include "utils/math_utils.h"

Sphere::Sphere(float radius) : radius(radius), center(glm::vec3(0))
{}

Sphere::Sphere(float radius, glm::vec3 center) : radius(radius), center(center)
{}

bool Sphere::rayIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, glm::vec3 *p0, glm::vec3 *p1) const
{
    // https://www.youtube.com/watch?v=HFPlKQGChpE
    glm::vec3 dir = glm::normalize(rayDirection);
    float t = glm::dot(center - rayOrigin, dir);
    glm::vec3 p = rayOrigin + dir * glm::vec3(t);

    float y = glm::length(center - p);

    if (y > radius) return false;

    float x = sqrt((radius * radius) - (y * y));

    if (p0)
    {
        glm::vec3 point0 = rayOrigin + dir * glm::vec3(t - x);

        p0->x = point0.x;
        p0->y = point0.y;
        p0->z = point0.z;
    }
    if (p1)
    {
        glm::vec3 point1 = rayOrigin + dir * glm::vec3(t + x);

        p1->x = point1.x;
        p1->y = point1.y;
        p1->z = point1.z;
    }
    return true;
}

SharedMesh Sphere::generate(const std::string &meshName, int sectorCount, int stackCount, VertAttributes &attrs)
{
    int nrOfVerts = (sectorCount + 1) * (stackCount + 1);
    SharedMesh mesh = SharedMesh(new Mesh(meshName, nrOfVerts, (sectorCount * (stackCount - 1)) * 6, attrs));

    int posOffset = attrs.getOffset(VertAttributes::POSITION);
    int norOffset = attrs.contains(VertAttributes::NORMAL) ? attrs.getOffset(VertAttributes::NORMAL) : -1;
    int texOffset = attrs.contains(VertAttributes::TEX_COORDS) ? attrs.getOffset(VertAttributes::TEX_COORDS) : -1;

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
    float tx, ty, tz;
    float s, t;                                     // texCoord

    float sectorStep = 2.f * M_PI / sectorCount;
    float stackStep = ((float) M_PI) / stackCount;
    float sectorAngle, stackAngle;

    int vertI = 0;
    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2.f - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        float lon = 360.0 * ((float)x / stackCount);

        tx = cosf(lon * M_PI / 180.0f - .5 * M_PI);
        ty = 0.f;
        tz = sinf(lon * M_PI / 180.0f - .5 * M_PI);

        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            mesh->set(glm::vec3(x, y, z), vertI, posOffset);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            mesh->set(glm::vec3(nx, ny, nz), vertI, norOffset);

            // vertex tex coord between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            mesh->set(glm::vec2(s, t), vertI, texOffset);
            vertI++;
        }
    }

    assert(nrOfVerts == vertI);

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;

    int i = 0;
    for(int x = 0; x < stackCount; ++x)
    {
        k1 = x * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int y = 0; y < sectorCount; ++y, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if(x != 0)
            {
                mesh->indices[i++] = k1;
                mesh->indices[i++] = k2;
                mesh->indices[i++] = k1+1;
            }

            if(x != (stackCount-1))
            {
                mesh->indices[i++] = k1 + 1;
                mesh->indices[i++] = k2;
                mesh->indices[i++] = k2+1;
            }
        }
    }
    assert(mesh->nrOfIndices == i);

    TangentCalculator::addTangentsToMesh(mesh);

    return mesh;
}