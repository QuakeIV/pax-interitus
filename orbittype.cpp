
#include "orbittype.h"
#include "celestialtype.h"
#include "universe.h"

OrbitType::OrbitType()
{
}

//TODO: non circular orbits (racetrack should support this, we just need to be able to take additional parameters)
OrbitType::OrbitType(CelestialType *p, int64_t r)
{
    parent = p;
    orbital_radius = r;
    //doing sine/cosine math in double land, and then converting back to the fixed reference frame
    //TODO: figure out how to do this right with integers (maybe just use bigint or something)
    double radius_d = r; //radius in MM

    // only define an orbit if there is a parent object
    if (parent != NULL)
    {
        for (int i = 0; i < racetrack_points; i++)
        {
            // TODO: stick with clockwise or flip?
            double angle = (2.0*PI*i)/racetrack_points;
            rel_racetrack[i].x = radius_d*cos(angle);
            rel_racetrack[i].y = radius_d*sin(angle);
        }

        //period in milliseconds (doing calculation in double land, and then converting back to the integral reference frame)
        const double G = 66743000; //gravitational constant, in cubic mm per exagram millisecond squared
        //did some algebra, assuming newtonian gravity, newtonian motion this should yield orbital period
        double period_d = (radius_d*PI*2.0*sqrt(((radius_d) / (p->mass * G))));
        orbital_period = (int64_t)period_d;
        //worst case roundoff error here is I think racetrack_points - 1 milliseconds (this is the true reference for orbital period in terms of calculating position)
        racetrack_delta_time = orbital_period/racetrack_points;
        //start the planet at a random spot in its orbit
        orbit_clock_offset = (int64_t)(QRandomGenerator::system()->bounded(period_d)); //TODO: make this not random by default?
    }
}

static int64_t muldiv(int64_t x, int64_t mul, int64_t div)
{
    __int128_t trueval = x;
    trueval*= mul;
    trueval/= div;
    return trueval;
}

void OrbitType::UpdatePosition(void)
{
    if (parent != NULL)
    {
        //TODO: avoid modulo by issuing updates to each planets time concept and then wrapping around
        int64_t orbit_clock = (universe_time + orbit_clock_offset) % orbital_period;
        int index  = (int)(orbit_clock/racetrack_delta_time);
        int next   = (index + 1) % racetrack_points; //wraps back around to 0 if we are about to overflow
        FixedV2D d = rel_racetrack[next] - rel_racetrack[index];

        d.x = muldiv(d.x, orbit_clock%racetrack_delta_time, racetrack_delta_time);
        d.y = muldiv(d.y, orbit_clock%racetrack_delta_time, racetrack_delta_time);

        position = d + rel_racetrack[index] + parent->trajectory.position;
    }
}
