#include "spacecraft.h"
#include "orbittype.h"
#include "components/component.h"
#include "components/jumpdrive.h"
#include "universe.h"

// NOTE: current data model holds that you should not make pointers of items in the spacecraft, as if the list re-allocates the addresses will change

SpacecraftDesign::SpacecraftDesign()
{

}
SpacecraftDesign::~SpacecraftDesign()
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
    // TODO: would be nice to not have to check this
//    if (selected_drive < 0 || selected_drive >= jump_drives.length())
//        return false;
//    return jump_drives[selected_drive]->jump(this, tgt);
    return true;
}
bool Spacecraft::jump(Transform* tgt, int selected_drive)
{
    // TODO: would be nice to not have to check this
    if (selected_drive < 0 || selected_drive >= jump_drives.length())
        return false;

    return jump_drives[selected_drive]->jump(this, tgt);
}
// max acceleration (mm/sec/sec)
int64_t Spacecraft::max_acceleration(void)
{
    return 0;
}
// damage in joules?
void Spacecraft::take_damage(uint64_t damage)
{
}
// drive regular operating of the craft
void Spacecraft::update(int64_t delta_t)
{
}
