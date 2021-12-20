#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "fixedv2d.h"

class Transform
{
public:
    Transform();
    ~Transform();

    virtual void UpdatePosition(void);

    FixedV2D position;
};

#endif // TRANSFORM_H
