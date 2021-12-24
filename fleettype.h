#ifndef FLEETTYPE_H
#define FLEETTYPE_H

#include "orbittype.h"
#include <QString>
#include <QColor>

class SolarSystemType;

class FleetType
{
public:
    OrbitType trajectory;

    static unsigned long fleet_id; //currently just used to initialize 'name'

    QColor color;

    QString name;

    FleetType(CelestialType *p, unsigned long r);
    ~FleetType();
};

#endif // FLEETTYPE_H
