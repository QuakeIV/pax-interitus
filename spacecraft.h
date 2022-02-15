#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "orbittype.h"
#include "components/component.h"
#include "universe.h"


class SpacecraftDesign
{
public:
    SpacecraftDesign()
    {

    }

    QList<Component*> all_components;

    QList<Component*>* selected_recharge_queue;
    QMap<QString, QList<Component*>> recharge_profiles;

    QList<Engine*> engines;

    QList<Reactor*> reactors;

    QList<DirectedWeapon*> directed_weapons;
    //TODO: missile launchers (which are not directed)

    // aught to be possible to have multiples, however i reckon there
    // should be engineering problems to overcome before multiples are practical
    QList<JumpDrive*> jump_drives;
    JumpDrive *selected_jump_drive; // you can only jump with one drive at a time, this tracks the currently selected drive you can attempt to jump with

    // to be initialized
    // TODO: this gets inherited by the Spacecraft class, retrofits should probably update this
    QString class_name = "Spacecraft Design";

    // max acceleration (mm/sec/sec)
    int64_t max_acceleration(void);

    int64_t max_range(void);

    // assume a spherical spacecraft/chicken (for now)
    // this should stay fixed after its manufactured and in spite of damage
    // TODO: use this for display purposes to make relative ship size within a formation evident?
    int64_t radius;
};

// indicates spacecraft status (non reciprocating)
class SpacecraftStatus
{
    // ship currently preparing for or undergoing subspace transit
    bool jumping = false;
};

// TODO: some concept of orientation and perhaps maneuvering thrusters?
class Spacecraft : public SpacecraftDesign
{
public:
    Spacecraft()
    {
        spacecraft.append(this);

        // assume that eventually recharge configurations would be loaded from a save file or such
        if (!selected_recharge_queue)
        {
            if (recharge_profiles.isEmpty())
            {
                QList<Component*> profile = recharge_profiles["default"] = QList<Component*>();
                foreach (Component *c, all_components)
                {
                    if (c->uses_power)
                        profile.append(c);
                }
                selected_recharge_queue = &profile;
            }
            else
            {
                selected_recharge_queue = &recharge_profiles.first();
            }
        }
    }
    ~Spacecraft()
    {
        spacecraft.removeOne(this);
    }

    bool ready_to_jump(void)
    {
        if (!selected_jump_drive)
            return false;

        return true;
    }
    bool jump(OrbitType* tgt)
    {
        if (!ready_to_jump())
            return false;

        return true;
    }
    bool select_jump_drive(JumpDrive *drive)
    {
        if (jump_drives.contains(drive))
        {
            selected_jump_drive = drive;
            return true;
        }
        return false;
    }

    // design that the spacecraft was most recently specced to
    SpacecraftDesign *design;

    // meant to drive the display of spacecraft status
    SpacecraftStatus status;

    // TODO: kinematic trajectory that can either ride a fixed orbit or be in a dynamic maneuvering mode
    // TODO: need some way to approach a target point and produce an ETA to that point (ideally closed form)
    OrbitType trajectory;

    // to be overidden
    QString name = "Spacecraft";

    // max acceleration (mm/sec/sec)
    int64_t max_acceleration(void)
    {
        return 0;
    }

    // damage in joules
    void take_damage(uint64_t damage)
    {
    }

    // drive regular operating of the craft
    void update(int64_t delta_t)
    {
        // update components before trying to use them
        foreach( Component *c, all_components)
            c->update(this);

        // do other stuff
    }
};

#endif // SPACECRAFT_H
