#include "celestial.h"
#include "solarsystemtype.h"

// for stars
Celestial::Celestial(int64_t r, uint64_t m)
{
    radius = r;
    mass = m;
    color = QColor(242,210,4); //reasonable shade of yellow (ideally this will get overridden during system gen)
    name = "Unnamed Star"; // set default name
    parent = NULL;

    //just use default constructor for trajectory, which will result in a do-nothing trajectory centered at 0,0
}

Celestial::Celestial(int64_t r, uint64_t m, int64_t distance, Celestial *p):
    trajectory(p, distance)
{
    parent = p;
    system = parent->system;
    radius = r;
    mass = m;
    
    color = QColor(180,180,180); //assume gray dusty moon until overridden

    name = parent->name + " " + QString::number(parent->children.length());
    parent->children.append(this);
    system->celestials.append(this);
}
