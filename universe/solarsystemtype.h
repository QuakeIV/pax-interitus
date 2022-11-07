#ifndef SOLARSYSTEMTYPE_H
#define SOLARSYSTEMTYPE_H

#include <QList>
#include "celestialtype.h"
#include "spacecraft/spacecraft.h"

class Taskgroup;
class SolarSystemType;
extern QList<SolarSystemType*> systems;

class SolarSystemType
{
public:
    CelestialType root; //root celestial body (this might possibly change later)

    QList<Spacecraft*> spacecraft;
    QList<CelestialType*> celestials;

    static uint64_t system_id; //currently just a counter used to initialize 'name'
    QString name;

    // parameters for root celestial object
    SolarSystemType(int64_t r, uint64_t m);
};

#endif // SOLARSYSTEMTYPE_H
