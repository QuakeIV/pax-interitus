#ifndef COMPONENT_H
#define COMPONENT_H

#include "utilities.h"
#include "transform.h"

class BeamType;
class Spacecraft;

// common properties of component designs
class ComponentDesign
{
    QString name;
};

class Component
{
protected:
    // where does the component currently live
    // TODO: parent class for spacecraft vs installations or other such things
    Spacecraft *parent;
    // component mass is in grams
    int64_t mass;

    // track joules of damage taken so far
    // the apply damage function will have an opportunity to absorb a degree of it as appropriate
    // ideally the update functions
    int64_t damage;
public:
    Component() {}

    // component mass is in grams (this function lets the component factor its current state into the mass result)
    virtual int64_t get_mass(void) { return mass; }

    // damage in joules for now
    // returns damage absorbed so it can fall through to other components
    // TODO: default HP system?
    // TODO: absorption of some sort (maybe temperature and pressure tolerance thing)
    virtual int64_t apply_damage(int64_t dmg) { return 0; };

    QString name;

    static const bool uses_power = false;
    static const bool produces_power = false;

    // this is meant to be called by the parent vessel to allow the component to function
    // or perhaps explode, or so forth
    virtual void update(Spacecraft *parent) {}

    // provide power to the component (in watts)
    virtual void charge(int64_t wattage) {}

    // TODO: more overrides for planetary defense facilities and so forth
    // (probably this will result in common callbacks for both types of thing, or maybe both get a common subtype?)
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
    static const bool produces_power = true;

    int64_t max_power_generation;
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
    virtual int64_t get_effective_range(void);

    // currently as a presumption towards a 2d projection of a 3d environment, rather than a truly 2d one, you can only
    // hit the target you were aiming for, hence requiring tgt as a second arg
    virtual void fire_on_target(FixedV2D aim, Spacecraft *tgt);
    // TODO: overloads for other kinds of targets

    // 'damage' at given range
    virtual int64_t damage_at_range(int64_t distance);
};

//TODO: invet active armor system that requires power to keep it running
//TODO: shields
//TODO: battlefield armor repair system?

// plasma beam cannon (currently instantaneous like all beams)
class BeamCannon : DirectedWeapon
{

public:
    BeamCannon(Transform *pos);
};

// missile launchers aught to work differently
// TODO: missile launcher
// TODO: missile (probably its own type and not a subtype of component, though really it should be a mini spacecraft in some sense)
// TODO: missile magazine (when not a box launcher)
// TODO: storage (supplies, cargo, same component?)

#endif // COMPONENT_H
