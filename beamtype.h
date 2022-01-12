#ifndef BEAMTYPE_H
#define BEAMTYPE_H

#include "fixedv2d.h"
#include "utilities.h"

// laser beam as such
// for now beams are assumed to be truly instantaneous (as are sensors pretty much)
//TODO: this should probably be spawned by a beam cannon type
class BeamType
{
public:
    inline BeamType(FixedV2D o, FixedV2D a)
    {
        origin = o; //point at which shot was fired
        aimpoint = a; //point of aim
        aim_length2 = (o - a).length2(); //length of aim arc, which can be pre-computed
        aim_length = int_sqrt(aim_length2);
    }

    // distance from line to specified point
    // TODO: may be cheaper to just stick with squared distance, then compare that with squared radius of target
    inline int64_t distance(FixedV2D p)
    {
        __uint128_t origin_target2 = p.distance2(origin);
        __uint128_t aim_target2 = p.distance2(aimpoint);
        // TODO: algorithm is not great in terms of managing overflows
        // also takes some effeciency hits by dividing more than needed to try to lessen the overflow problem somewhat
        // TODO: there is also an unfortuante degree of roundoff, this just typically amounts to a few millimeters so meh
        __uint128_t intermediate = ((origin_target2 / aim_length) >> 1) + (aim_length >> 1) - ((aim_target2 / aim_length) >> 1);
        __uint128_t d2 = origin_target2 - intermediate * intermediate;
        return int_sqrt(d2);
    }

    // beam radius in mm, this is currently meant to be the radius at the target (which may shift based on distance)
    int64_t radius;

protected:
    FixedV2D origin; // where shot originated from
    FixedV2D aimpoint; // point of aim (weapon inaccuracy is to be baked into this quantity)
    int64_t aim_length; // length of point of origin->aim
    __uint128_t aim_length2; // squared length of point of origin->aim
    int64_t damage; // damage at position of target
};

#endif // BEAMTYPE_H
