#ifndef EFFECTS_H
#define EFFECTS_H

#include "fixedv2d.h"

typedef enum effect_e
{
    effect_base = 0,

} effect_t;

// effects exist purely to cue rendering of things
// will be newed up and need to be destroyed
class Effect
{
public:
    Effect()
    {

    }

    FixedV2D loc;

    // return true if the time has come to yeet this particular effect out of existence
    bool update(void)
    {
        return true;
    }
};

#endif // EFFECTS_H
