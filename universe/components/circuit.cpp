#include "circuit.h"
#include "component.h"
#include "spacecraft.h"
#include "materials.h"
#include "universe.h"


void Circuit::update(Spacecraft *parent)
{
    // this should probably remain a stub until we add reactions to damage (which should set the universe step time appropriately so this doesn't miss the chance to react to that)
}
