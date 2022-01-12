
#include "component.h"
#include "beamtype.h"
#include "spacecraft.h"

Component::Component()
{

}

// component mass is in grams
int64_t Component::get_mass(void)
{
    return mass;
}


Engine::Engine()
{

}


Reactor::Reactor()
{

}


DirectedWeapon::DirectedWeapon(Transform *pos)
{
    position = pos;
}

int64_t DirectedWeapon::get_effective_range(void)
{
    return 0;
}

void DirectedWeapon::fire_on_target(Transform *aim, Spacecraft *tgt)
{
    // create beam from current position to point of aim
    BeamType b = BeamType(position->position, aim->position);
    b.radius = radius; //TODO: logic for changing bema radius on distance (beam divergence)

    tgt->handle_beam_hit(&b);
}


BeamCannon::BeamCannon(Transform *pos):
    DirectedWeapon(pos)
{
}
