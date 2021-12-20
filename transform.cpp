#include "transform.h"
#include "universe.h"
#include <stdio.h>

Transform::Transform()
{
    printf("NEW TRANSFORM REE\n");
    transforms.append(this);
}

Transform::~Transform()
{
    // shouldnt wind up in here multiple times anyhow, thus try to save a bit of time by yeeting only one
    transforms.removeOne(this);
}

void Transform::UpdatePosition()
{
    // do nothing
}
