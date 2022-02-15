
#include "component.h"
#include "spacecraft.h"

Component::Component()
{

}
// component mass is in grams
int64_t Component::get_mass(void)
{
    return mass;
}
// damage in joules for now
// returns damage absorbed so it can fall through to other components
int64_t Component::apply_damage(int64_t dmg)
{
    return 0;
}
// this is meant to be called by the parent vessel to allow the component to function
void Component::update(Spacecraft *parent)
{

}


Engine::Engine()
{

}


JumpDrive::JumpDrive()
{

}


Reactor::Reactor()
{

}


DirectedWeapon::DirectedWeapon(Transform *pos)
{
}
// describe effective range (parameters defining what 'effective' means to come)
int64_t DirectedWeapon::get_effective_range(void)
{
    return 0;
}
int64_t DirectedWeapon::damage_at_range(int64_t distance)
{
    // TODO: adjust this later for overlapping radii area plus beam divergence
    return base_damage;
}
void DirectedWeapon::fire_on_target(FixedV2D aim, Spacecraft *tgt)
{
    // distance from line to specified point
    // TODO: may be cheaper to just stick with squared distance, then compare that with squared radius of target
    // origin: point shot was fired from
    // poa: point of aim
    // target: position of target
    __uint128_t aim_length2 = (parent->trajectory.position - aim).length2(); //length of aim arc
    int64_t aim_length = int_sqrt(aim_length2);

    __uint128_t origin_target2 = tgt->trajectory.position.distance2(parent->trajectory.position);
    __uint128_t aim_target2 = tgt->trajectory.position.distance2(aim);
    // TODO: algorithm is not great in terms of managing overflows
    // also takes some effeciency hits by dividing more than needed to try to lessen the overflow problem somewhat
    // TODO: there is also an unfortuante degree of roundoff, this just typically amounts to a few millimeters so meh
    __uint128_t intermediate = ((origin_target2 / aim_length) >> 1) + (aim_length >> 1) - ((aim_target2 / aim_length) >> 1);
    __uint128_t d2 = origin_target2 - intermediate * intermediate;
    int64_t distance_from_beam = int_sqrt(d2);

    if (distance_from_beam < (tgt->radius + distance_from_beam))
    {
        //TODO: overlapping circles math to calculate partial yield
        tgt->take_damage(int_sqrt(origin_target2));
    }
}


BeamCannon::BeamCannon(Transform *pos):
    DirectedWeapon(pos)
{
}
