#ifndef JUMPDRIVE_H
#define JUMPDRIVE_H

#include "component.h"
#include "capacitor.h"

// TODO: it would i think be nice for these to be specialized to particular spacecraft designs and integral to them
class JumpdriveDesign : public ComponentDesign
{
    CapacitorDesign cap;
    static const bool uses_power = true;
};

// advanced propulsion component
class Jumpdrive : public Component
{
public:
    JumpdriveDesign design;
    Capacitor cap;

    void update(Spacecraft *parent) override
    {
        //cap.update(parent);
    }
};

#endif // JUMPDRIVE_H
