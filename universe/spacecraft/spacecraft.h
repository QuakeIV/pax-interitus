#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "orbittype.h"
#include "components/component.h"
#include "components/circuit.h"
#include "universe.h"
#include <QList>

class JumpdriveDesign;
class Jumpdrive;

class Component;

// TODO: parent class for spacecraft vs installations or other such things
class SpacecraftDesign
{
public:
    SpacecraftDesign();
    ~SpacecraftDesign()
    {

    }

    bool operator==(const SpacecraftDesign *rhs) const
    {
        return rhs == const_cast<SpacecraftDesign*>(this);
    }

    // IE Cv for corvette
    // TODO: might be worth expanding on that a bit so both Cv and Corvette are know info for different contexts
    QString type = "";
    QString class_name = "";

    QList<EngineDesign*> engines;
    QList<ReactorDesign*> reactors;
    QList<DirectedweaponDesign*> directed_weapons;
    QList<JumpdriveDesign*> jump_drives;
    QList<CircuitDesign*> circuits;

    // max acceleration (mm/sec/sec)
    int64_t max_acceleration(void);

    int64_t max_range(void);

    // assume a spherical spacecraft/chicken (for now)
    // this should stay fixed after its manufactured and in spite of damage
    // TODO: use this for display purposes to make relative ship size within a formation evident?
    int64_t radius;
};

// produce full name of a spacecraft design
// ie Cv Deimos Class
// TODO: incorporate empire specific info
inline QString get_spacecraft_design_name(SpacecraftDesign *d)
{
    if (!d)
        return "No Design";
    if (d->class_name.isEmpty())
        return "New Class";
    else
        return d->class_name + " Class";
}

// indicates spacecraft status (non reciprocating)
class SpacecraftStatus
{
    // ship currently preparing for or undergoing subspace transit
    bool jumping = false;
};

// TODO: parent class for spacecraft vs installations or other such things
// TODO: some concept of orientation and perhaps maneuvering thrusters?
class Spacecraft
{
public:
    Spacecraft();
    ~Spacecraft();
    
    // design that the spacecraft was most recently specced to
    SpacecraftDesign design;

    bool ready_to_jump(void);
    bool jump(Transform* tgt, int selected_drive);

    QList<Engine*> engines;
    QList<Reactor*> reactors;
    QList<Directedweapon*> directed_weapons;
    //TODO: missile launchers

    // aught to be possible to have multiples, however i reckon there
    // should be engineering problems to overcome before multiples are practical
    QList<Jumpdrive*> jump_drives;

    // meant to drive the display of spacecraft status
    SpacecraftStatus status;

    // TODO: kinematic trajectory that can either ride a fixed orbit or be in a dynamic maneuvering mode
    // TODO: need some way to approach a target point and produce an ETA to that point (ideally closed form)
    Transform *trajectory;

    QString name = "";
    int hull_number;

    // max acceleration (mm/sec/sec)
    int64_t max_acceleration(void);

    // TODO: this may turn out to be a bit trash
    // damage in joules
    void take_damage(uint64_t damage);

    // drive regular operating of the craft
    void update(int64_t delta_t);
};

#endif // SPACECRAFT_H
