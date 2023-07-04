#include "celestial.h"
#include "solarsystem.h"

// for stars
Celestial::Celestial(double r, uint64_t m, SolarSystem *system):
    position() // just 0,0
{
    radius = DISTANCE_M_TO_FIXED(r);
    mass = m;
    color = QColor(242,210,4); //reasonable shade of yellow (ideally this will get overridden during system gen)
    name = "Unnamed Star"; // set default name
    parent = NULL;
    // TODO: kindof risky, no way to guard this at the moment afaik
    // TODO: add null check to python for deref-assignment members to check for this
    trajectory = NULL;

    surface_gravity = G * CELESTIALMASS_TO_KG(m) / (r*r);

    //just use default constructor for trajectory, which will result in a do-nothing trajectory centered at 0,0
}

Celestial::Celestial(double r, uint64_t m, double distance, Celestial *p):
    position() // just 0,0
{
    parent = p;
    system = parent->system;
    radius = DISTANCE_M_TO_FIXED(r);
    mass = m;
    trajectory = new Orbit(p, distance, &position);

    surface_gravity = G * CELESTIALMASS_TO_KG(m) / (r*r);
    
    color = QColor(180,180,180); //assume gray dusty moon until overridden

    name = parent->name + " " + QString::number(parent->children.length());
    parent->children.append(this);
    system->celestials.append(this);
}
