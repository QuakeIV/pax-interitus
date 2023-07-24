#include "solarsystem.h"
#include "universe.h"
#include "celestial.h"

uint64_t SolarSystem::system_id = 0;

SolarSystem::SolarSystem(double r, double m)
{
    //TODO: not optimal way to initialize the child type, maybe just have a default constructor for celestials?
    root = new Celestial(r, m, this);
    systems.append(this);

    name = "System " + QString::number(system_id++);
}
