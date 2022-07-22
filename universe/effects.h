#ifndef EFFECTS_H
#define EFFECTS_H

#include "orbittype.h"

// effects exist purely to cue rendering of things
// TODO: which list the effect lives in will define what it looks like
// TODO: maybe provide a generic field to customize it somewhat?
class Effect
{
public:
    Effect()
    {

    }

    OrbitType position;

    int64_t creation_time;
    int64_t destruction_time;
    int64_t radius; // in standard distance reference frame (mm)
};

#endif // EFFECTS_H
