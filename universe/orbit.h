#ifndef ORBIT_H
#define ORBIT_H

#include "fixedv2d.h"
#include <math.h>
#include <QRandomGenerator>

#define PI (3.14159265358979)

class SolarSystem;
class Celestial;
class Orbit;

extern QList<Orbit*> trajectories;

//TODO: create a subtype 'fixed orbit' for planets and a different more expensive to compute type for fleets when they are on the move
class Orbit
{
    FixedV2D get_position_at_time(int64_t time);

public:
    SolarSystem *solarsystem   = NULL;
    FixedV2D *position         = NULL;
    Celestial *parent          = NULL; //for now assume you can only really orbit a celestial TODO: improve?
    int64_t orbital_period     = 0; //in fixed time
    int64_t orbit_clock_offset = 0; //in fixed time
    int64_t orbital_radius     = 0; //in fixed distance (this is currently only used for trajectory render culling)

    Orbit(Celestial *p, double r, FixedV2D *pos);
    // copy constructor
    Orbit(Orbit *o, FixedV2D *pos);
    ~Orbit(void);

    void update_position(void);
    FixedV2D project_position(int64_t delta_time);

    //256 seems to be a good number of points for now
    // NOTE: must be a power of 2
    static const int racetrack_points = 256;
    static const int racetrack_mask = racetrack_points - 1;
    FixedV2D rel_racetrack[racetrack_points];

private:
    int64_t racetrack_delta_time; //in microseconds
};

#endif // ORBIT_H
