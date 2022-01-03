#ifndef BEAMTYPE_H
#define BEAMTYPE_H

#include "fixedv2d.h"

// laser beam as such
// for now beams are assumed to be truly instantaneous (as are sensors pretty much)
//TODO: this should probably be spawned by a beam cannon type
class BeamType
{
public:
    inline BeamType(FixedV2D o, FixedV2D a)
    {
        origin = o;
        aimpoint = a;
    }

    // distance from line to specified point
    inline int64_t distance(FixedV2D p)
    {
        // TODO: algorithm is not great in terms of overflows
//        __uint128_t d2 =
        return 0;
    }

private:
    FixedV2D origin; // where shot originated from
    FixedV2D aimpoint; // point of aim (weapon inaccuracy is to be baked into this quantity)
    int64_t aim_length; // length of point of origin->aim
    __uint128_t aim_length2; // squared length of point of origin->aim
};

#endif // BEAMTYPE_H
