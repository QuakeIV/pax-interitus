#include "spacecraft.h"
#include "beamtype.h"

Spacecraft::Spacecraft()
{

}

int64_t Spacecraft::max_acceleration(void)
{
    //TODO: this will be derived from components and in particular the damage state of said components
    return 0;
}

bool Spacecraft::detect_beam_hit(BeamType *beam)
{
    return beam->distance(trajectory.position) < radius;
}
