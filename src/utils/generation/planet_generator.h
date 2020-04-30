#ifndef PLANET_GENERATOR_H
#define PLANET_GENERATOR_H

#include <functional>
#include <glm/glm.hpp>

#include "common/planet.hpp"
#include "common/land_mass.hpp"
#include "utils/math/polygon.h"
#include "geometry/model.hpp"
#include "landmass_generator.h"

#define IslandContextProvider std::function<IslandContext(int islandNr)>

struct IslandContext
{

    IslandGenerator islandGenerator;
    float minLatitude, maxLatitude;

};


class PlanetGenerator
{

  public:

    PlanetGenerator(Planet *planet, IslandContextProvider islContextProvider, int nrOfIslands);

    void generate();

  private:

    IslandContextProvider islContextProvider;

    int nrOfIslands, minNrOfIslands;
    Planet *plt;

    bool tryToGenerate();
    
    bool placeOnPlanet(LandMass *isl, float minLat, float maxLat);

    bool tryToPlaceOnPlanet(LandMass *isl, float lon, float lat);

    bool overflowsLongitude(LandMass* isl);

    bool overlaps(LandMass* isl0, LandMass* isl1);

};

#endif
