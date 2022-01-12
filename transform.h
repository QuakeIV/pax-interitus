#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "fixedv2d.h"

class Transform;
extern QList<Transform*> transforms;

class Transform
{
public:
    Transform()
    {
        transforms.append(this);
    }

    ~Transform()
    {
        // shouldnt wind up in here multiple times anyhow, thus try to save a bit of time by yeeting only one
        transforms.removeOne(this);
    }

    virtual void UpdatePosition(void) {}

    FixedV2D position;
};

#endif // TRANSFORM_H
