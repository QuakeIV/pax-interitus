#ifndef FLEETTYPE_H
#define FLEETTYPE_H

#include "orbittype.h"
#include <QString>
#include <QColor>
#include "spacecraft.h"
#include "celestialtype.h"
#include "solarsystemtype.h"

// TODO: order handling infrastructure

class FleetType
{
public:
    OrbitType trajectory;

    static uint64_t fleet_id; //currently just a counter used to initialize 'name'

    QColor color;

    QString name;

    QList<Spacecraft*> ships;

    FleetType(CelestialType *p, int64_t r):
        trajectory(p, r)
    {
        color = QColor(Qt::yellow);

        name = "Task Force " + QString::number(fleet_id++);

        p->system->fleets.append(this);
    }

    ~FleetType()
    {
        // shouldnt get redundant entries, in which case this is an acceptable optimization to not scan the whole list
        trajectory.parent->system->fleets.removeOne(this);
    }
};

#endif // FLEETTYPE_H
