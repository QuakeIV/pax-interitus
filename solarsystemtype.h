#ifndef SOLARSYSTEMTYPE_H
#define SOLARSYSTEMTYPE_H

#include "fleettype.h"
#include <QList>
#include "celestialtype.h"

class CelestialType;

class SolarSystemType
{
public:
    CelestialType root; //root celestial body (this might possibly change later)

    QList<FleetType*> fleets;
    QList<CelestialType*> celestials;

    // parameters for root celestial object
    SolarSystemType(int64_t r, uint64_t m);
};

#endif // SOLARSYSTEMTYPE_H
