#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "materials.h"
#include "universe.h"

class Spacecraft;
class ComponentDesign;
class Component;

// what is edited in the designer window
// probably all components will have a special design type, i cant forsee any actual overlap there (unlike the components themselves)
class CircuitDesign
{
public:
    bool operator==(const CircuitDesign *rhs) const
    {
        return rhs == const_cast<CircuitDesign*>(this);
    }
    virtual QString descriptor_string()
    {
        return name;
    }

    QString name = "Circuit Design";
    Insulator *insulator;
    Conductor *conductor;

    double rated_voltage;
    double rated_amperage;

    QList<ComponentDesign*> components;

    // TODO:
    // overall size of spacecraft dictates average weight dedicated to circuit (maybe just diameter times cross section of different materials)
};

// this represents the machinery in a spacecraft dedicated to transmitting power between components on a given circuit
// specifies voltage, max current, material of conductor and insulator
// insulator can break down due to heat
// TODO: capacitor-like description of temperature rising and falling based on convection dissipation vs tolerances of materials
class Circuit
{
public:
    bool operator==(const Circuit *rhs) const
    {
        return rhs == const_cast<Circuit*>(this);
    }
    virtual QString descriptor_string()
    {
        return design.name;
    }

    CircuitDesign design;

    // TODO: jury rigs would go into this list but not the design?
    QList<Component*> components;

    // stolen from smlease.com/calculator/heat-transfer/convective-heat-transfer/
    // assume air convection for now, whatever (liquid cooling later?)
    // W = heatcoeffecient * area * deltatemperature
    // heatcoeffecient naturally is 5-25 W/(m^2K) in air without forced air flow (which could also maybe be designable later?)
    // heatcoeffecient = 10.45 - v + 10sqrt(v) for airflow velocity

    void update(Spacecraft *parent);
};

#endif // CIRCUIT_H
