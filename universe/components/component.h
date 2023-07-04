#ifndef COMPONENT_H
#define COMPONENT_H

#include "utilities.h"
#include "orbit.h"
#include "circuit.h"

class BeamType;
class Spacecraft;

// common properties of component designs
class ComponentDesign
{
public:
    virtual QString descriptor_string()
    {
        return name;
    }

    QString name = "component design";

    // TODO: i dont think this actually makes sense, circuit designs are ship specific
    // component designs are not necessarily bound to a particular ship right?
    // not symmetrical to components in that way
    CircuitDesign *circuit;

    static const bool uses_power = false;
    static const bool produces_power = false;
};

class Component
{
public:
    virtual QString descriptor_string()
    {
        return name;
    }

    QString name;
    // where does the component currently live
    Spacecraft *parent;

    // component mass is in grams
    int64_t mass;

    // track joules of damage taken so far
    // the apply damage function will have an opportunity to absorb a degree of it as appropriate
    // TODO: ideally the update functions update this
    int64_t damage;

    Component() {}

    // damage in joules for now
    // returns damage absorbed so it can fall through to other components
    // TODO: default HP system?
    // TODO: absorption of some sort (maybe temperature and pressure tolerance thing)
    // TODO: it turns out vtables are a bit gay should probably discontinue the use thereof as much as possible
    virtual int64_t apply_damage(int64_t dmg) { return 0; }

    // TODO: this is generally dynamic, maybe this will just be updated as needed?
    double resistive_load = 0;

    // this is meant to be called by the parent vessel to allow the component to function
    // or perhaps explode, or so forth
    virtual void update(Spacecraft *parent) {}

    // circuit will calculate supply voltage based on intended circuit voltage, load, and max rated wattage
    // circuit shall take care of not re-calling this if the voltage remains unchanged
    virtual void update_voltage(double voltage) {}

    // TODO: more overrides for planetary defense facilities and so forth
    // (probably this will result in common callbacks for both types of thing, or maybe both get a common subtype?)
};

// propulsion component
class EngineDesign : public ComponentDesign
{

};

class Engine : public Component
{
public:
    EngineDesign design;
};

// power generation component
// TODO: per calculating loads, we could store a max load in terms of resistance to toggle between regular operation and voltage drop modes
// eh note that should probably be at the circuit level, the reactor should just have a wattage and voltage rating, and then be notified of its current voltage
class ReactorDesign : public ComponentDesign
{
public:
    static const bool produces_power = true;
};
class Reactor : public Component
{
public:
    ReactorDesign design;

    int64_t max_power_generation;
};

class DirectedweaponDesign : public ComponentDesign
{
public:
    static const bool uses_power = true;
};

class Directedweapon : public Component
{
protected:
    int64_t radius; // mm of radius of emitted firepower
    int64_t base_damage; // base damage

public:
    DirectedweaponDesign design;

    Directedweapon();

    // in mm, this is probably going to change over time but something should be provided to characterize effective range
    // most likely will have parameters at some point
    // TODO: liable to be used for things like displaying the expected effective range of a beam weapon, or drive AI firing behavior
    // may also need a notion of separating when the weapon stops doing much damage from the idea of it being able to hit accurately
    // TODO: maybe more like preferred range?
    virtual int64_t get_effective_range(void);

    // currently as a presumption towards a 2d projection of a 3d environment, rather than a truly 2d one, you can only
    // hit the target you were aiming for, hence requiring tgt as a second arg
    virtual void fire_on_target(FixedV2D aim, Spacecraft *tgt);
    // TODO: overloads for other kinds of targets

    // 'damage' at given range
    virtual int64_t damage_at_range(int64_t distance);
};

//TODO: active armor system that requires power to keep it running?
//TODO: shields
//TODO: battlefield armor repair system?

// TODO: i am now thinking this should probably actually amount to a parameterization of the base directed weapon class
// plasma beam cannon (currently instantaneous like all beams)
//class BeamCannon : DirectedWeapon
//{

//public:
//    BeamCannon(Transform *pos);
//};

// missile launchers aught to work differently
// TODO: missile launcher
// TODO: missile (probably its own type and not a subtype of component, though really it should be a mini spacecraft in some sense)
// TODO: missile magazine (when not a box launcher)
// TODO: storage (supplies, cargo, same component?) (probably not exactly)

#endif // COMPONENT_H
