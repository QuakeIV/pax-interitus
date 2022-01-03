#include "fleettype.h"
#include "celestialtype.h"
#include "solarsystemtype.h"

uint64_t FleetType::fleet_id = 0;

FleetType::FleetType(CelestialType *p, int64_t r):
    trajectory(p, r)
{
    color = QColor(Qt::yellow);

    name = "Task Force " + QString::number(fleet_id++);

    p->system->fleets.append(this);
}

FleetType::~FleetType()
{
    // shouldnt get redundant entries, in which case this is an acceptable optimization
    trajectory.parent->system->fleets.removeOne(this);
}
