#ifndef ORBITTYPE_H
#define ORBITTYPE_H

#include "transform.h"
#include <math.h>
#include <QRandomGenerator>

#define PI (3.14159265358979)

class CelestialType;

//TODO: create a subtype 'fixed orbit' for planets and a different more expensive to compute type for fleets when they are on the move
class OrbitType : public Transform
{
public:
    int64_t orbital_period     = 0; //in microseconds
    int64_t orbit_clock_offset = 0; //initial position, in microseconds
    int64_t orbital_radius     = 0; //radius in mm (this is currently only used for trajectory render culling)
    CelestialType *parent      = NULL; //for now assume you can only really orbit a celestial TODO: improve?

    OrbitType(CelestialType *p, int64_t r);
    OrbitType();

    void UpdatePosition(void) override;

    static const int racetrack_points = 128; //128 seems to be a good number of points for now
    FixedV2D rel_racetrack[racetrack_points];

private:
    int64_t racetrack_delta_time; //in microseconds
};

#endif // ORBITTYPE_H
