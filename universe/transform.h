#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "fixedv2d.h"

class Transform;
class SolarSystem;

extern QList<Transform*> transforms;

class Transform
{
public:
    Transform(SolarSystem *solarsystem);

    ~Transform(void);

    SolarSystem *solarsystem;

    virtual void update_position(void);

    // project position (to be overidden by different sub types of transform)
    FixedV2D project_position(int64_t delta_t);

    FixedV2D position;
};

#endif // TRANSFORM_H
