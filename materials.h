#ifndef MATERIALS_H
#define MATERIALS_H

#include <stdint.h>
#include <QString>
#include "utilities.h"

class Material
{
public:
    // kg per liter
    double density;
    // TODO: tie this to an empire's knowledge of dialectric materials instead
    QString name;

    // TODO: tie this to an empire's knowledge of dialectric materials instead
    virtual QString descriptor_string()
    {
        return name;
    }
};

// TODO: add mithril?

class Dialectric : public Material
{
    //TODO: it appears a relevant metric might be 'minimum thickness'
public:
    // absolute dialectric (farads/meter)
    double permittivity;
    // volts per meter
    double strength;

    // TODO: tie this to an empire's knowledge of dialectric materials instead
    QString descriptor_string() override
    {
        return "(" + get_voltage_str(strength) + "/m)(" + get_capacitance_str(permittivity) + "/m) " + name;
    }
};

#endif // MATERIALS_H
