#include "solarsystem.h"
#include "universe.h"

uint64_t SolarSystem::system_id = 0;

SolarSystem::SolarSystem(double r, double m):
  root(r,m,this) //TODO: not optimal way to initialize the child type, maybe just have a default constructor for celestials?
{
    root.system = this;
    celestials.append(&root);
    systems.append(this);

    name = "System " + QString::number(system_id++);
}
