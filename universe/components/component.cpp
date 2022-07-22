
#include "component.h"
#include "spacecraft.h"


Directedweapon::Directedweapon(Transform *pos)
{
}
// describe effective range (parameters defining what 'effective' means to come)
int64_t Directedweapon::get_effective_range(void)
{
    return 0;
}
int64_t Directedweapon::damage_at_range(int64_t distance)
{
    // TODO: adjust this later for overlapping radii area plus beam divergence
    return base_damage;
}
// directedweapon has to at least partially live in a C file because its a component and its dealing with the spacecraft type, so something has to break the include loop
void Directedweapon::fire_on_target(FixedV2D aim, Spacecraft *tgt)
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

    if (distance_from_beam < (tgt->design.radius + distance_from_beam))
    {
        //TODO: overlapping circles math to calculate partial yield
        tgt->take_damage(int_sqrt(origin_target2));
    }
}
