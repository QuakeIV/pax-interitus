#include "spacecraft.h"
#include "orbittype.h"
#include "components/component.h"
#include "components/jumpdrive.h"
#include "universe.h"

SpacecraftDesign::SpacecraftDesign()
{

}
// max acceleration (mm/sec/sec)
int64_t SpacecraftDesign::max_acceleration(void)
{
    return 0;
}
int64_t SpacecraftDesign::max_range(void)
{
    return 0;
}




Spacecraft::Spacecraft()
{
    spacecraft.append(this);
}
Spacecraft::~Spacecraft()
{
    spacecraft.removeOne(this);
}
bool Spacecraft::ready_to_jump(void)
{
    if (!selected_jump_drive)
        return false;

    return true;
}
bool Spacecraft::jump(OrbitType* tgt)
{
    if (!ready_to_jump())
        return false;

    //TODO: spawn jump effects that then render?
    return true;
}
bool Spacecraft::select_jump_drive(uint drive)
{
    if (drive >= jump_drives.count())
        return false;

    selected_jump_drive = &jump_drives[drive];
    return true;
}
// max acceleration (mm/sec/sec)
int64_t Spacecraft::max_acceleration(void)
{
    return 0;
}
// damage in joules
void Spacecraft::take_damage(uint64_t damage)
{
}
// drive regular operating of the craft
void Spacecraft::update(int64_t delta_t)
{
    // update components before trying to use them
    foreach( Component *c, components)
        c->update(this);

    // do other stuff
}
