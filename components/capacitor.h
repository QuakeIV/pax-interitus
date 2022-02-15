#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "component.h"
#include "universe.h"

// power storage component (stores charge for devices that require the sudden release of energy)
class Capacitor : Component
{
    int64_t initial_time = 0;
    // TODO: consider power of 2 division
    // getting the math to perfectly converge on this would require backend work however
    // to perhaps for instance make the time units base 2 fractions of a second or so forth

public:
    Capacitor() {}

    static const bool uses_power = true;

    void update(Spacecraft *parent) override
    {
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
    void charge(int64_t rate)
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
