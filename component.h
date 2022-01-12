#ifndef COMPONENT_H
#define COMPONENT_H

#include "utilities.h"
#include "transform.h"

class BeamType;
class Spacecraft;

class Component
{
protected:
    // where does the component currently live
    Transform *position;
    // component mass is in grams
    int64_t mass;
public:
    Component();

    // component mass is in grams
    int64_t get_mass(void);

};

// propulsion component
class Engine : Component
{
public:
    Engine();
};

// power generation component
class Reactor : Component
{
public:
    Reactor();
};

class DirectedWeapon : Component
{
protected:
    int64_t radius; // mm of radius of emitted firepower
    int64_t base_damage; // base damage

public:
    DirectedWeapon(Transform *pos);

    // in mm, this is probably going to change over time but something should be provided to characterize effective range
    // most likely will have parameters at some point
    // TODO: liable to be used for things like displaying the expected effective range of a beam weapon, or drive AI firing behavior
    // may also need a notion of separating when the weapon stops doing much damage from the idea of it being able to hit accurately
    int64_t get_effective_range(void);

    // currently as a presumption towards a 2d projection of a 3d environment, rather than a truly 2d one, you can only
    // hit the target you were aiming for, hence requiring tgt as a second arg
    void fire_on_target(Transform *pos, Spacecraft *tgt);
    // TODO: overloads for other kinds of targets

    // 'damage' at target, these units are undefined but are intended to be rough equivalents of energy
    int64_t damage(Spacecraft *tgt)
    {
        // TODO: adjust this later for overlapping radii area plus beam divergence
        return base_damage;
    }
};

// plasma beam cannon (currently instantaneous like all beams)
class BeamCannon : DirectedWeapon
{

public:
    BeamCannon(Transform *pos);
};

// missile launchers aught to work differently


#endif // COMPONENT_H
