#include "transform.h"

Transform::Transform(SolarSystem *solarsystem)
{
    transforms.append(this);
    this->solarsystem = solarsystem;
}

Transform::~Transform(void)
{
    // convergent towards not being duplicated on accident
    transforms.removeAll(this);
}

void Transform::update_position(void)
{
}

    // project position (to be overidden by different sub types of transform)
FixedV2D Transform::project_position(int64_t delta_t)
{
    return position;
}
