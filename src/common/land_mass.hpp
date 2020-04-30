#pragma once

// Standard Headers
#include <vector>
#include <memory>
#include <map>

// Local Headers
#include "utils/math/polygon.h"
#include "geometry/mesh.hpp"
#include "graphics/camera.hpp"


class Planet;
// class BuildingsSystem;

static const float ISLAND_ROTATION = 30;

class LandMass
{

  public:
    Planet *planet;
    int width, height, nrOfVerts;
    float longitude, latitude;
    glm::mat4 planetTransform;

    bool isInView = true;

    float seaBottom = 0;

    SharedMesh terrainMesh;

    LandMass(int width, int height, Planet *plt);

    // from binary:
    LandMass(const std::vector<uint8> &data, uint32 dataOffset, Planet *plt);

    void toBinary(std::vector<uint8> &out) const;

    int xyToVertI(int x, int y) const;

    int vertIToX(int i) const;

    int vertIToY(int i) const;

    bool tileAtSeaFloor(int x, int y) const;

    bool tileAboveSea(int x, int y) const;

    float tileSteepness(int x, int y) const;

    float distToHeight(int x, int y, float minHeight, float maxHeight, int maxDist) const;

    bool containsLonLatPoint(float lon, float lat) const;

    bool containsTile(int x, int y) const;

    vec3 tileCenter(int x, int y) const;

    // returns 0-1 based on amount of vertices that are underwater
    float percentageUnderwater() const;

    std::vector<vec3>
        // normal per vertex in local space
        vertexNormals,

        // normal per vertex
        vertexNormalsPlanet,

        // vertex positions in local space, including curvature of the planet.
        vertexPositions,

        // vertex positions in 'planet'/world space, including curvature of the planet.
        vertexPositionsPlanet,

        // vertex positions in local space, EXCLUDING curvature.
        vertexPositionsOriginal;

    std::vector<Polygon>
        // outlines spherically projected on the planet using Longitude & Latitude
        outlinesLongLat,

        // outlines in 2d. (As if the planet was flat)
        outlines2d;

    std::vector<std::vector<vec3>>
        // outlines in 3d.
        outlines3d,
        // outlines in 3d transformed.
        outlines3dTransformed;

    /**
     * Texture map of the island.
     * An island can have 5 textures.
     * 1 background texture and 4 extra textures.
     *
     * Each vertex has a vec4.
     * 
     * The n-th float of that vec4 tells how much of n-th texture should be rendered at the position of that vertex.
     */
    std::vector<vec4> textureMap;

    /**
     * The following functions were previously in (island/planet)_generator.h
     * But these functions are now also needed by the planet_loader
     */
    void planetDeform();

    void calculateNormals();

    void createMesh();

    void transformVertices();

    void placeOnPlanet();


  private:

    void transformOutlines();

    void calculateLatLonOutlines();
};