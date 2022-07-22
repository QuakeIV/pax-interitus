
#include "orbittype.h"
#include "celestialtype.h"
#include "universe.h"
#include "utilities.h"
#include "units.h"

OrbitType::OrbitType()
{
}

//TODO: non circular orbits (racetrack should support this, we just need to be able to take additional parameters)
OrbitType::OrbitType(CelestialType *p, int64_t r)
{
    parent = p;
    orbital_radius = r;
    //doing sine/cosine math in double land, and then converting back to the fixed reference frame
    //TODO: figure out how to do this right with integers
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
        //TODO: 1 << TIME_SHIFT factor is to compensate for microsecond time
        double period_d = (radius_d*PI*2.0*sqrt(((radius_d * (double)TIME_FACTOR) / (p->mass * G))));
        orbital_period = (int64_t)period_d;
        //worst case roundoff error here is I think racetrack_points - 1 milliseconds (this is the true reference for orbital period in terms of calculating position)
        racetrack_delta_time = orbital_period/racetrack_points;
        //start the planet at a random spot in its orbit
        //TODO: make this not random by default? (or at least procedurally random)
        orbit_clock_offset = (int64_t)(QRandomGenerator::system()->bounded(period_d));
    }
}

FixedV2D OrbitType::get_position_at_time(int64_t time)
{
    if (parent != NULL)
    {
        //TODO: avoid modulo by tracking each planets time concept and then wrapping around
        //(i think the ideal case is to remember when our current orbit started and then subtract)
        int64_t orbit_clock = (universe_time + orbit_clock_offset) % orbital_period;
        int index  = (int)(orbit_clock/racetrack_delta_time);
        int next   = (index + 1) % racetrack_points; //wraps back around to 0 if we are about to overflow (avoid modulo here as well)
        FixedV2D d = rel_racetrack[next] - rel_racetrack[index];

        d.x = muldiv(d.x, orbit_clock%racetrack_delta_time, racetrack_delta_time);
        d.y = muldiv(d.y, orbit_clock%racetrack_delta_time, racetrack_delta_time);

        return d + rel_racetrack[index] + parent->trajectory.position;
    }
    return position;
}

FixedV2D OrbitType::project_position(int64_t delta_time)
{
    return get_position_at_time(universe_time + delta_time);
}

void OrbitType::update_position(void)
{
    position = get_position_at_time(universe_time);
}
