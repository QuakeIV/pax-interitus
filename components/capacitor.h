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
    // in micrometers, for now (special local distance reference frame, not as yet standard)
    int64_t plate_separation = 0;
    // plate area (sq mm)
    int64_t plate_area = 0;
    // inline resistor milliohms
    int64_t resistance = 1;

    // returns millivolts
    int64_t max_voltage(void)
    {
        // funnily enough this actually appears to work out as millivolts
        return dialectric->strength * plate_separation;
    }

    // returns millifarads
    int64_t capacitance(void)
    {
        if (plate_separation <= 0)
            return 0;
        // *1000 for farads to millifarads
        return (dialectric->permittivity * plate_area * 1000) / plate_separation;
    }

    // takes voltage in millivolts
    // returns joules
    int64_t max_energy(__int128_t voltage)
    {
        // TODO: need a standard way to go into power of 2 divisors of true values, so that division is avoided
        // /1000 converting millivolts to volts, vs dealing with millifarads
        return (((__int128_t)capacitance() * voltage * voltage) >> 1) / 1000;
    }

    // takes voltage in millivolts
    // returns milliamps (TBR)
    int64_t max_current(int64_t voltage)
    {
        if (resistance <= 0)
            return 0;
        // *1000 for amps to milliamps
        return (1000*voltage) / resistance;
    }

    // returns microseconds
    int64_t charge_time(void)
    {
        // /1000 for milliohms to ohms, 1000 for millifarads to farads, *1000000 to get to microseconds
        return (5 * resistance * capacitance());
    }

    //TODO: weight, volume
};

// power storage component (stores charge for devices that require the sudden release of energy)
class Capacitor : Component
{
    CapacitorDesign *design;

    int64_t initial_time = 0;
    // TODO: consider power of 2 division
    // getting the math to perfectly converge on this would require backend work however
    // to perhaps for instance make the time units base 2 fractions of a second or so forth

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
            // factor of 1000000 is converting seconds of charge time to microseconds
            initial_time = (stored_energy * 1000000) / rate;
            max_dt = (max_energy * 1000000) / rate;
            discharge_dt = (discharge_energy * 1000000) / rate;
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
