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

    virtual void update_position(void) { qDebug() << "ERROR: Transform update position was called"; }

    // project position (to be overidden by different sub types of transform)
    FixedV2D project_position(int64_t delta_t)
    {
        return position;
    }

    FixedV2D position;
};

#endif // TRANSFORM_H
