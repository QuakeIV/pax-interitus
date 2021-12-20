#ifndef ORBITTYPE_H
#define ORBITTYPE_H

#include "transform.h"
#include <math.h>
#include <QRandomGenerator>

#define PI (3.14159265358979)

class CelestialType;

class OrbitType : public Transform
{
public:
    long  orbital_period         = 0; //in milliseconds
    long  orbit_clock_offset     = 0; //initial position, in milliseconds
    CelestialType *parent        = NULL; //for now assume you can only really orbit a celestial TODO: improve?

    OrbitType(CelestialType *p, unsigned long r);
    OrbitType();

    void UpdatePosition(void) override;

    static const int racetrack_points = 64; //64 seems to be a good number of points for now
    FixedV2D rel_racetrack[racetrack_points];

private:
    long racetrack_delta_time; //in milliseconds
};

#endif // ORBITTYPE_H
