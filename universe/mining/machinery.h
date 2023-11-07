#ifndef MINING_MACHINERY_H
#define MINING_MACHINERY_H

#include <stdint.h>

// represents a spec for mining machinery
class MiningMachineryDesign
{
public:
    MiningMachineryDesign(void);
    ~MiningMachineryDesign(void);

    // TODO: actually do this? eh
    //   i mean, it could add some depth if it were an aspect of the deposits
    // surface area used
    // will serve to differentiate machinery, where very widely/thinly spread deposits will favore different families of machine
    // probably something like you can favorably trade off against footprint/kg to get more output per kg, or lower cost, or any of the other attributes
    //double footprint_per_kg;

    // kg/s
    // ie mass flow rate per machine
    double extraction_rate;

    // kg/s
    // rate at which machine consumes supplies while producing minerals at the set rate
    double supply_rate;

    // TODO: passive supply consumption, actual machinery weardown/ability to run without supply?

    // TODO: size? (mass/volume)
};

#endif // MINING_MACHINERY_H
