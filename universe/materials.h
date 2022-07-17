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
    // joules per kg degree K, how many joules are required to heat up one kg of this material by one degree
    double specific_heat = 1.0; //avoid div by 0
    // TODO: empire should be able to rename materials within their own knowledge space
    // therefore you can avoid annoyance of people not knowing what material they are dealing with
    QString name;

    // TODO: tie this to an empire's knowledge of dialectric materials, rather than the actual properties
    virtual QString descriptor_string()
    {
        return name;
    }
};

// TODO: add mithril?

class Insulator : public Material
{
    //TODO: it appears a relevant metric might be 'minimum thickness'
public:
    // absolute dialectric constant (farads/meter)
    double permittivity;
    // volts per meter, dialectric strength
    double strength;

    // TODO: tie this to an empire's knowledge of materials instead of the actual material representation within the universe
    QString descriptor_string() override
    {
        return "(" + get_voltage_str(strength) + "/m)(" + get_capacitance_str(permittivity) + "/m) " + name;
    }
};

class Conductor : public Material
{
    // ohm-meters
    double resistivity;
};

#endif // MATERIALS_H
