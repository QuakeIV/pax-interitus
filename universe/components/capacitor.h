#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "component.h"
#include "materials.h"
#include "universe.h"

// what is edited in the designer window
// probably all components will have a special design type, i cant forsee any actual overlap there (unlike the components themselves)
class CapacitorDesign : public ComponentDesign
{
public:
    // TODO: factor in conductor material?
    // TODO: factor in insulating sheathing on outer surface to prevent arcing?

    // dielectric sandwich material
    Insulator *insulator;
    // meters
    double plate_separation = 0;
    // square meters
    double plate_area = 0;
    // inline resistor ohms
    double resistance = 1;

    //TODO: track spec voltage, which will inform when it starts to take damage
    //TODO: spec amperage as well
    //TODO: eventual failures due to mismatch between spec and reality?

    // returns volts
    double max_voltage(void)
    {
        return insulator->strength * plate_separation;
    }

    // returns farads
    double capacitance(void)
    {
        if (plate_separation <= 0)
            return 0;
        return (insulator->permittivity * plate_area) / plate_separation;
    }

    // returns joules
    double energy_at_voltage(double voltage)
    {
        return (capacitance() * voltage * voltage) * (1/2.0);
    }

    // amps
    double max_current(double voltage)
    {
        if (resistance <= 0)
            return 0;
        return voltage / resistance;
    }

    // cubic meters
    // TODO: ships should fairly ineffeciently use volume, with this being a factor related to the design itself
    // TODO: move to component parent class
    double volume(void)
    {
        return CUBIC_METER_TO_LITER(plate_separation * plate_area);
    }

    // kilograms
    // TODO: ships should use mass with perfect efficiency
    // TODO: move to component parent class
    double mass(void)
    {
        return volume() * insulator->density;
    }

    // returns standard reference frame time (in delta T, not absolute)
    int64_t charge_time(void)
    {
        // 5 for 'five time units' which we shall assume is de facto capacitor charging time
        // TODO: might be a better fit to go for 3 time units or so?
        return (5.0 * resistance * capacitance() * TIME_FACTOR);
    }

    //TODO: weight, volume
};

// power storage component (stores charge for devices that require the sudden release of energy)
class Capacitor : Component
{
    CapacitorDesign design;

public:
    Capacitor(void)
    {
    }

    static const bool uses_power = true;

    // returns joules per second instead of joules per 'time unit'
    double get_current_wattage()
    {
        return current_charge_rate * TIME_FACTOR;
    }

    int64_t get_stored_energy()
    {
        if (initial_energy > current_max_energy)
            return initial_energy;

        if (universe_time > charge_time)
            return current_max_energy;

        // TODO: this still feels like a really unfortunate compromise in every way
        // it might be better to just accept energy as a double
        int64_t dt = universe_time - initial_time;
        int64_t e = (int64_t)(((double)dt) * current_charge_rate) + initial_energy;
        if (e > initial_energy)
            return e;
        return initial_energy;
    }

    void update(Spacecraft *parent) override
    {
        // this should probably remain a stub until we add reactions to damage (which should set the universe step time appropriately so this doesn't miss the window on that)
    }
    int64_t initial_time = 0;
    int64_t initial_energy = 0;
    double current_charge_rate = 0; // de facto this is joules per 'time unit' since thats cheaper for math purposes and its a double so that works out fine precision wise
    double current_voltage = 0;
    int64_t current_max_energy = 0; // energy
    int64_t max_dt = 0; // time it takes to fully charge (currently no way this should change after initialization (maybe later it will degrade due to damage or other factors)
    int64_t charge_time = 0; // time at which the capacitor will be fully charged
    int64_t discharge_energy = 0; // amount of energy consumed per discharge (defined by containing component generally)

    //NOTE: this will cope very poorly with rapidly fluctuating voltage
    void update_voltage(double voltage) override
    {
        // TODO: react to over-volt

        if (current_voltage == voltage)
            return;

        initial_energy = get_stored_energy();

        // update max storeable energy (this function is needed for the designer so its shared)
        current_max_energy = design.energy_at_voltage(voltage);
        current_charge_rate = ((double)current_max_energy) / ((double)max_dt); // energy shift minus time shift, targeting wattage shift

        //TODO: later consider if its better to just back-compute an 'initial time' in the past and to then use universe_time + max_dt for charge time
        initial_time = universe_time;
        charge_time = universe_time + (int64_t)((current_max_energy - initial_energy) / current_charge_rate);
    }

    // true = charged
    bool charged(void)
    {
        int64_t energy = get_stored_energy();
        if (energy >= discharge_energy)
            return true;
        return false;
    }

    // for now this is simply an instantaneous action
    // TODO: there may one day be a need to have this take some amount of time, if small
    // might be a thing tech can effect
    // TODO: could probably save cycles by having the containing LRU schedule events if it wants to fire
    // in that case might be able to avoid the check? probably still need to recalculate to subtract out energy that was used
    bool discharge(void)
    {
        int64_t energy = get_stored_energy();

        if (energy >= discharge_energy)
        {
            initial_energy -= discharge_energy;
            initial_time = universe_time;
            charge_time = universe_time + (int64_t)((current_max_energy - initial_energy) / current_charge_rate);
            return true;
        }
        return false;
    }
};

#endif // CAPACITOR_H
