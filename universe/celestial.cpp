#include "celestial.h"
#include "solarsystem.h"
#include "mining/deposit.h"

// for stars
Celestial::Celestial(double r, double m, SolarSystem *s):
    position() // just 0,0
{
    radius = DISTANCE_M_TO_FIXED(r);
    mass = KG_TO_FIXEDMASS(m);
    color = QColor(242,210,4); //reasonable shade of yellow (ideally this will get overridden during system gen)
    name = "Unnamed Star"; // set default name
    parent = NULL;
    system = s;
    system->celestials.append(this);

    // TODO: kindof risky, no way to guard this at the moment afaik
    // TODO: add null check to python for deref-assignment members to check for this
    // no trajectory will result in remaining stationary at 0,0
    trajectory = NULL;
    // TODO: star mineralogy
    mineralogy = NULL;

    surface_gravity = G * m / (r*r);
}

Celestial::Celestial(double r, double m, double distance, Celestial *p):
    position() // just 0,0
{
    parent = p;
    system = parent->system;
    radius = DISTANCE_M_TO_FIXED(r);
    mass = KG_TO_FIXEDMASS(m);
    trajectory = new Orbit(p, distance, &position);

    mineralogy = generate_mineralogy(mass);

    surface_gravity = G * m / (r*r);

    color = QColor(180,180,180); //assume gray dusty moon until overridden

    name = parent->name + " " + QString::number(parent->children.length());
    parent->children.append(this);
    system->celestials.append(this);
}
