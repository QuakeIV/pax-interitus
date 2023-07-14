#include "spacecraft.h"
#include "orbit.h"
#include "components/component.h"
#include "components/jumpdrive.h"
#include "universe.h"

// NOTE: current data model holds that you should not make pointers of items in the spacecraft, as if the list re-allocates the addresses will change

SpacecraftDesign::SpacecraftDesign(void)
{

}
SpacecraftDesign::~SpacecraftDesign(void)
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


static uint64_t hull_number_autoincrement = 0;

Spacecraft::Spacecraft(Orbit *o)
{
    spacecraft.append(this);

    trajectory = new Orbit(o, &position);
    hull_number = hull_number_autoincrement++;
}

Spacecraft::Spacecraft(Celestial *p, double r)
{
    spacecraft.append(this);

    trajectory = new Orbit(p, r, &position);
    hull_number = hull_number_autoincrement++;
}
Spacecraft::~Spacecraft(void)
{
    spacecraft.removeAll(this);
}
bool Spacecraft::ready_to_jump(Orbit *tgt)
{
    if (jump_drives.contains(selected_drive))
        return selected_drive->cap.charged(selected_drive->calculate_jump_energy(this, tgt));
    return true;
}
bool Spacecraft::jump(Orbit *tgt)
{
    // TODO: would be nice to not have to check this
    if (jump_drives.contains(selected_drive))
        return selected_drive->jump(this, tgt);

    return false;
}
bool Spacecraft::select_jumpdrive(Jumpdrive *drive)
{
    if (jump_drives.contains(drive))
    {
        selected_drive = drive;
        return true;
    }
    return false;
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
