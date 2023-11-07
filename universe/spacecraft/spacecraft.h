#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "orbit.h"
#include "components/component.h"
#include "components/circuit.h"
#include "universe.h"
#include <QList>

class JumpdriveDesign;
class Jumpdrive;
class Celestial;

// TODO: parent class for spacecraft vs installations or other such things
class SpacecraftDesign
{
public:
    SpacecraftDesign(void);
    ~SpacecraftDesign(void);

    // IE Cv for corvette
    // TODO: might be worth expanding on that a bit so both Cv and Corvette are know info for different contexts
    QString type = "";
    QString class_name = "";

    // TODO: probably want to either track this differently, or separate 'design' from a concept of a 'spec'
    // where the 'spec' is tracked at the empire level
    // actually 'spec' could be a wrapper for the empire level that contains a design, and is separate from spacecraft entirely
    // this would go there probably
    bool obsolete;

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

// TODO: one thing that would be really cool is the ship getting blow'd up and spraying debris that actually show up on the screen flying off at various angles with their own trajectories
// TODO: parent class for spacecraft vs installations or other such things
// TODO: some concept of orientation and perhaps maneuvering thrusters?
class Spacecraft
{
public:
    Spacecraft(Celestial *p, double r);
    Spacecraft(Orbit *o);
    ~Spacecraft(void);
    
    // design that the spacecraft was most recently specced to
    SpacecraftDesign design;

    Jumpdrive *selected_drive = NULL;

    bool ready_to_jump(Orbit *tgt);
    bool jump(Orbit *tgt);
    bool select_jumpdrive(Jumpdrive *drive);

    QList<Engine*> engines;
    QList<Reactor*> reactors;
    QList<Directedweapon*> directed_weapons;
    //TODO: missile launchers

    // aught to be possible to have multiples, however i reckon there
    // should be engineering problems to overcome before multiples are practical
    QList<Jumpdrive*> jump_drives;

    // TODO: need some way to approach a target point and produce an ETA to that point (ideally closed form)
    FixedV2D position;
    Orbit *trajectory;

    QString name = "";
    // TODO: is this really wanted or needed in this form?
    uint64_t hull_number;

    // max acceleration (mm/sec/sec)
    int64_t max_acceleration(void);

    // TODO: this may turn out to be a bit trash
    // damage in joules
    void take_damage(uint64_t damage);
};

#endif // SPACECRAFT_H
