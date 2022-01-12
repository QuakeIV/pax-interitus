#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "orbittype.h"
#include "beamtype.h"


class SpacecraftDesign
{
public:
    SpacecraftDesign()
    {

    }

    // to be overidden
    QString class_name = "Spacecraft Design";

    // max acceleration (mm/sec/sec)
    int64_t max_acceleration(void);

    int64_t max_range(void);

    // assume a spherical spacecraft/chicken (for now)
    // this should stay fixed after its manufactured and in spite of damage
    // TODO: use this for display purposes to make relative ship size within a formation evident?
    int64_t radius;
};

class Spacecraft : SpacecraftDesign
{
public:
    Spacecraft()
    {

    }

    // TODO: kinematic trajectory that can either ride a fixed orbit or be in a dynamic maneuvering mode
    // TODO: need some way to approach a target point and produce an ETA to that point (ideally closed form)
    OrbitType trajectory;

    // to be overidden
    QString name = "Spacecraft";

    // max acceleration (mm/sec/sec)
    int64_t max_acceleration(void)
    {
        return 0;
    }

    void handle_beam_hit(BeamType *beam)
    {
        if (beam->distance(trajectory.position) < radius)
        {

        }
    }
};

#endif // SPACECRAFT_H
