#include "celestialtype.h"
#include "solarsystemtype.h"

// for stars
CelestialType::CelestialType(unsigned long r, unsigned long m)
{
    radius = r;
    mass = m;
    color = QColor(242,210,4); //reasonable shade of yellow (ideally this will get overridden during system gen)
    name = "Unnamed Star"; // set default name

    //just use default constructor for trajectory, which will result in a do-nothing trajectory centered at 0,0
}

CelestialType::CelestialType(unsigned long r, unsigned long m, unsigned long distance, CelestialType *parent)
{
    system = parent->system;
    radius = r;
    mass = m;

    color = QColor(180,180,180); //assume gray dusty moon until overridden
    trajectory = OrbitType(parent, distance);
    name = parent->name + " " + QString::number(parent->children.length());
    parent->children.append(this);
    system->celestials.append(this);
}
