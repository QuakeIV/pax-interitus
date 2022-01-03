#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "orbittype.h"

class BeamType;

class Spacecraft
{
public:
    Spacecraft();

    OrbitType trajectory;

    // max acceleration (mm/sec/sec)
    double max_acceleration;

    bool detect_beam_hit(BeamType *beam);
};

#endif // SPACECRAFT_H
