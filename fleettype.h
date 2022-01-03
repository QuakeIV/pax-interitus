#ifndef FLEETTYPE_H
#define FLEETTYPE_H

#include "orbittype.h"
#include <QString>
#include <QColor>
#include "spacecraft.h"

class SolarSystemType;

class FleetType
{
public:
    OrbitType trajectory;

    static uint64_t fleet_id; //currently just a counter used to initialize 'name'

    QColor color;

    QString name;

    QList<Spacecraft*> ships;

    FleetType(CelestialType *p, int64_t r);
    ~FleetType();
};

#endif // FLEETTYPE_H
