#ifndef JUMPDRIVE_H
#define JUMPDRIVE_H

#include "component.h"
#include "capacitor.h"

// advanced propulsion component
class JumpDrive : Component
{
public:
    JumpDrive() {};

    Capacitor cap;

    void update(Spacecraft *parent) override
    {
        cap.update(parent);
    }

    static const bool uses_power = true;
};

#endif // JUMPDRIVE_H
