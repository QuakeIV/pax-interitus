#ifndef JUMPDRIVE_H
#define JUMPDRIVE_H

#include "component.h"
#include "capacitor.h"

class JumpdriveDesign : ComponentDesign
{

    static const bool uses_power = true;
};

// advanced propulsion component
class Jumpdrive : Component
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
