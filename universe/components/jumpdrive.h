#ifndef JUMPDRIVE_H
#define JUMPDRIVE_H

#include "component.h"
#include "capacitor.h"
#include "spacecraft/spacecraft.h"
#include "transform.h"
#include "orbittype.h"

// TODO: it would i think be nice for these to be specialized to particular spacecraft designs and integral to them
class JumpdriveDesign : public ComponentDesign
{
    CapacitorDesign cap;
    static const bool uses_power = true;
};

// advanced propulsion component
class Jumpdrive : public Component
{
public:
    JumpdriveDesign design;
    Capacitor cap;

    int64_t calculate_jump_energy(Spacecraft *parent, Transform *target)
    {
        //energy = parent.mass * some factor * jump distance?
        // actually, KE delta accounts for mass
        // this is weird though because then its free (even profitable?) to jump deeper into a well, that needs work
        return 0;
    }

    // NOTE: i think technically multiple things can share a transform just fine, but probably shouldnt
    // in other words, ideally *target is a new transform usually
    // TODO: just going to be instant for now, but a discharge/cycle time would in general be an important mechanic
    // IE defining how quickly you can be out of a bad situation after deciding to leave
    bool jump(Spacecraft *parent, Transform *target)
    {
        if (cap.discharge(calculate_jump_energy(parent, target)))
        {
            delete parent->trajectory;
            parent->trajectory = target;

            //TODO: spawn jump effects that then render
            // this could be part of a 'jump signature' that actually is a mechanic in the game and persists for longer than it is usually visible in a decaying form

            return true;
        }
        return false;
    }

    void update(Spacecraft *parent) override
    {
        //cap.update(parent);
    }
};

#endif // JUMPDRIVE_H
