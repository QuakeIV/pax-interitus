
#include "orbit.h"
#include "celestial.h"
#include "universe.h"
#include "utilities.h"
#include "units.h"

// copy constructor
Orbit::Orbit(Orbit *o, FixedV2D *pos)
{
    *this = *o;
    position = pos;
    trajectories.append(this);
}

//TODO: non circular orbits (racetrack should support this, we just need to be able to take additional parameters)
Orbit::Orbit(Celestial *p, double r, FixedV2D *pos)
{
    parent = p;
    position = pos;
    solarsystem = p->system;
    orbital_radius = DISTANCE_M_TO_FIXED(r);
    trajectories.append(this);
    //doing sine/cosine math in double land, and then converting back to the fixed reference frame
    //TODO: figure out how to do this right with integers
    double radius_d = r; //radius in meters

    // only define an orbit if there is a parent object
    if (parent != NULL)
    {
        for (int i = 0; i < racetrack_points; i++)
        {
            // TODO: stick with clockwise or flip?
            double angle = (2.0*PI*i)/racetrack_points;
            rel_racetrack[i].x = DISTANCE_M_TO_FIXED(radius_d*cos(angle));
            rel_racetrack[i].y = DISTANCE_M_TO_FIXED(radius_d*sin(angle));
        }

        //period in milliseconds (doing calculation in double land, and then converting back to the integral reference frame)
        const double G = 0.0000000000667408; //gravitational constant (standard SI unit frame)
        //did some algebra, assuming newtonian gravity, newtonian motion this should yield orbital period
        double period_d = (radius_d*PI*2.0/sqrt((CELESTIALMASS_TO_KG(p->mass) * G)/radius_d)) * (double)TIME_FACTOR;
        orbital_period = (int64_t)period_d;
        //worst case roundoff error here is I think racetrack_points - 1 milliseconds (this is the true reference for orbital period in terms of calculating position)
        racetrack_delta_time = orbital_period/racetrack_points;
        //start the planet at a random spot in its orbit
        //TODO: make this not random by default? (or at least procedurally random)
        orbit_clock_offset = (int64_t)(QRandomGenerator::system()->bounded(period_d));
    }
}

Orbit::~Orbit(void)
{
    // TODO: when we custom allocate this will hopefully become a lot more efficient
    trajectories.removeAll(this);
}

FixedV2D Orbit::get_position_at_time(int64_t time)
{
    if (parent != NULL)
    {
        //TODO: avoid modulo by tracking each planets time concept and then wrapping around
        //(i think the ideal case is to remember when our current orbit started and then subtract)
        int64_t orbit_clock = (universe_time + orbit_clock_offset) % orbital_period;
        int index  = (int)(orbit_clock/racetrack_delta_time);
        // mask handles wraparound, this is why racetrack points must be powers of 2
        int next   = (index + 1) & racetrack_mask;
        FixedV2D d = rel_racetrack[next] - rel_racetrack[index];

        d.x = muldiv(d.x, orbit_clock%racetrack_delta_time, racetrack_delta_time);
        d.y = muldiv(d.y, orbit_clock%racetrack_delta_time, racetrack_delta_time);

        return d + rel_racetrack[index] + parent->position;
    }
    return FixedV2D();
}

FixedV2D Orbit::project_position(int64_t delta_time)
{
    return get_position_at_time(universe_time + delta_time);
}

void Orbit::update_position(void)
{
    *position = get_position_at_time(universe_time);
}

