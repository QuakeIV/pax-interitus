#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "orbittype.h"

class BeamType;

class Spacecraft
{
public:
    Spacecraft();

    // TODO: kinematic trajectory that can either ride a fixed orbit or be in a dynamic maneuvering mode
    OrbitType trajectory;

    // to be overidden
    QString name = "Spacecraft";

    // max acceleration (mm/sec/sec)
    int64_t max_acceleration(void);

    // assume a spherical spacecraft/chicken (for now)
    // this should stay fixed after its manufactured and in spite of damage
    // TODO: use this for display purposes to make relative ship size within a formation evident?
    int64_t radius;

    bool detect_beam_hit(BeamType *beam);
};

#endif // SPACECRAFT_H
