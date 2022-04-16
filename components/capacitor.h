#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "component.h"
#include "materials.h"

// what is edited in the designer window
// probably all components will have a special design type, i cant forsee any actual overlap there (unlike the components themselves)
class CapacitorDesign : public ComponentDesign
{
public:
    // dielectric sandwich material
    Dialectric *dialectric;
    // meters
    int64_t plate_separation = 0;
    // square meters
    int64_t plate_area = 0;
    // inline resistor ohms
    int64_t resistance = 1;

    // returns volts
    int64_t max_voltage(void)
    {
        return (dialectric->strength * plate_separation) >> ((DIALECTRIC_STRENGTH_SHIFT + PRECISION_DISTANCE_SHIFT) - VOLTAGE_SHIFT);
    }

    // returns farads
    int64_t capacitance(void)
    {
        if (plate_separation <= 0)
            return 0;
        return ((dialectric->permittivity * plate_area) / plate_separation) >> ((DIALECTRIC_CONSTANT_SHIFT + PRECISION_AREA_SHIFT - PRECISION_DISTANCE_SHIFT) - CAPACITANCE_SHIFT);
    }

    // returns joules
    int64_t max_energy(__int128_t voltage)
    {
        return ((((__int128_t)capacitance() * voltage * voltage) >> 1) >> ((CAPACITANCE_SHIFT + VOLTAGE_SHIFT + VOLTAGE_SHIFT) - ENERGY_SHIFT));
    }

    // amps
    int64_t max_current(int64_t voltage)
    {
        if (resistance <= 0)
            return 0;
        return ((voltage << (AMPERAGE_SHIFT)) / resistance) >> (VOLTAGE_SHIFT - RESISTANCE_SHIFT);
    }

    // returns standard reference frame time (in delta T, not absolute)
    int64_t charge_time(void)
    {
        // 5 for 'five time units' which we shall assume is de facto capacitor charging time
        return (5 * resistance * capacitance()) >> ((RESISTANCE_SHIFT + CAPACITANCE_SHIFT) - TIME_SHIFT);
    }

    //TODO: weight, volume
};

// power storage component (stores charge for devices that require the sudden release of energy)
class Capacitor : Component
{
    CapacitorDesign *design;

    int64_t initial_time = 0;

public:
    Capacitor() {}

    static const bool uses_power = true;

    void update(Spacecraft *parent) override
    {
        extern int64_t universe_time;
        // track initial_time to offset of current time if fully charged, so that charge stays consistent with reality
        if (initial_time + max_dt > universe_time)
        {
            initial_time = universe_time - max_dt;
            stored_energy = max_energy;
        }
        else
        {
            // TODO: would be nice to not do this all too often (in fairness once we are fully charged we are set for life)
            // thats probably going to turn out to nuke the majority of the unwanted performance impact so we are probably fine
            if (current_charge_rate)
                stored_energy = current_charge_rate * (universe_time - initial_time);
        }
    }
    int64_t current_charge_rate = 0;
    int64_t max_energy = 0;
    int64_t max_dt = 0; // time it takes to fully charge
    int64_t discharge_energy = 0; // amount of energy consumed per discharge
    int64_t discharge_dt = 0; // time it takes at current charge rate to regain one discharge
    int64_t stored_energy = 0;

    // charge rate in watts (for now)
    void charge(int64_t rate) override
    {
        if (current_charge_rate == rate)
            return;

        // reject negative numbers
        // *backflip* (this actually should be a faster negative number check)
        if (rate & (~LLONG_MAX))
        {
            qDebug() << "ERROR: trying to charge capacitor by negative rate";
            rate = 0;
        }

        //TODO: max charge rate?

        // TODO: might be a way to share this among multiple capacitors, maybe this can live at the circuit level somehow?
        if (rate)
        {
            // converting seconds of charge time to time units
            initial_time = (stored_energy << TIME_SHIFT) / rate;
            max_dt = (max_energy << TIME_SHIFT) / rate;
            discharge_dt = (discharge_energy << TIME_SHIFT) / rate;
        }
        current_charge_rate = rate;
    }

    // for now this is simply an instantaneous action
    // TODO: there may one day be a need to have this take some amount of time, if small
    // might even be a thing tech can effect
    bool discharge(void)
    {
        extern int64_t universe_time;
        if ((universe_time - initial_time) > discharge_dt)
        {
            initial_time += discharge_dt;
            stored_energy -= discharge_energy;
            return true;
        }

        return false;
    }
};

#endif // CAPACITOR_H
