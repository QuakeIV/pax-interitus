#include "solarsystemtype.h"

SolarSystemType::SolarSystemType(unsigned long r, unsigned long m):
  root(r,m), //TODO: not optimal way to initialize the child type, maybe just have a default constructor for celestials?
  fleets()
{
    root.system = this;
    celestials.append(&root);
}
