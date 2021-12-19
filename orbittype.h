#ifndef ORBITTYPE_H
#define ORBITTYPE_H

#include "transform.h"
#include "celestialtype.h"
#include <QList>
#include <math.h>
#include <QRandomGenerator>

#define PI (3.14159265358979)

QRandomGenerator qrand;

class OrbitType : public Transform
{
public:
    unsigned long orbital_radius = 0; //in mm, ideally later replaced with more parameters later
    long  orbital_period         = 0; //in milliseconds
    long  orbit_clock_offset     = 0; //initial position, in milliseconds

    OrbitType *parent; //object which this object orbits (null if it is root of a system)
    CelestialType *body; //object itself (contains mass, null if this is an effectively massless object)

    inline OrbitType()
    {
        OrbitType(NULL, 0);
    }

    inline OrbitType(CelestialType *c)
    {
        OrbitType(NULL, 0);
        body = c;
        c->parent = this;
    }

    inline OrbitType(OrbitType *p, unsigned long r, CelestialType *c)
    {
        OrbitType(p, r);
        body = c;
        c->parent = this;
    }

    inline OrbitType(OrbitType *p, unsigned long r)
    {
        parent = p;
        orbital_radius = r;
        body = NULL;

        // only define an orbit if there is a parent object
        if (parent != NULL)
        {
            //doing sine/cosine math in double land, and then converting back to the fixed reference frame
            //TODO: figure out how to do this right with integers
            double radius_d = (double) orbital_radius;
            for (int i = 0; i < racetrack_points; i++)
            {
                double angle = (2.0*PI*i)/racetrack_points;
                rel_racetrack[i].x = radius_d*cos(angle);
                rel_racetrack[i].y = radius_d*sin(angle);
            }

            //period in milliseconds (doing calculation in double land, and then converting back to the integral reference frame)
            const double G = 66743000; //gravitational constant, in cubic mm per exagram millisecond squared
            //did some algebra, assuming newtonian gravity, newtonian motion this should yield orbital period
            double period_d = (radius_d*PI*2.0*sqrt(((radius_d) / (p->body->mass * G))));
            orbital_period = (long)period_d;
            //worst case roundoff error here is I think racetrack_points - 1 milliseconds (this is the true reference for orbital period in terms of calculating position)
            racetrack_delta_time = orbital_period/racetrack_points;
            //start the planet at a random spot in its orbit
            orbit_clock_offset = (long)(qrand.bounded(period_d));
        }
    }

    inline OrbitType* AddChild(unsigned long r)
    {
        OrbitType *c = new OrbitType(this, r);
        children.append(c);
        return c;
    }

private:
    static const int racetrack_points = 64; //64 seems to be a good number of points for now
    long racetrack_delta_time; //in milliseconds
    FixedV2D rel_racetrack[racetrack_points];
    QList<OrbitType*> children;
};

#endif // ORBITTYPE_H
