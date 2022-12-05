#include "solarsystemtype.h"
#include "universe.h"

uint64_t SolarSystemType::system_id = 0;

SolarSystemType::SolarSystemType(double r, uint64_t m):
  root(r,m) //TODO: not optimal way to initialize the child type, maybe just have a default constructor for celestials?
{
    root.system = this;
    celestials.append(&root);
    systems.append(this);

    name = "System " + QString::number(system_id++);
}
