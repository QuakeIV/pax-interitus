#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "component.h"
#include "materials.h"
#include "universe.h"

// what is edited in the designer window
// probably all components will have a special design type, i cant forsee any actual overlap there (unlike the components themselves)
class CircuitDesign : public ComponentDesign
{
    // TODO:
    // spec voltage (insulator weight)
    // spec amperage (conductor weight)
    // conductor material?
    // insulator material?
    // power drawing components on circuit
    // generators on circuit
    // overall size of spacecraft dictates average weight dedicated to circuit (maybe just diameter times cross section of different materials)


};

// power storage component (stores charge for devices that require the sudden release of energy)
class Circuit : Component
{
    CircuitDesign *design;

public:
    Circuit()
    {
        assert(design!=NULL); // TODO: kill by defining NDEBUG
        
    }

    void update(Spacecraft *parent) override
    {
        // this should probably remain a stub until we add reactions to damage (which should set the universe step time appropriately so this doesn't miss the window on that)
    }
};

#endif // CIRCUIT_H
