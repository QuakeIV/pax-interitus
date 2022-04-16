#ifndef MATERIALS_H
#define MATERIALS_H

#include <stdint.h>
#include <QString>
#include "utilities.h"

class Material
{
public:
    int64_t density;
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
public:
    // absolute dialectric (farads/meter)
    int64_t permittivity;
    // volts per meter
    int64_t strength;

    // TODO: tie this to an empire's knowledge of dialectric materials instead
    QString descriptor_string() override
    {
        return "(" + get_voltage_str(strength) + "/m)(" + get_capacitance_str(permittivity) + "/m) " + name;
    }
};

#endif // MATERIALS_H
