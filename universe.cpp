#include "universe.h"
#include "transform.h"
#include "celestialtype.h"
#include "fleettype.h"
#include "solarsystemtype.h"

bool universe_paused = false;
long universe_time_warp = 0; //this is a power applied to 2
long universe_time;

//TODO: it might be better to only update the transforms we are currently looking at (cull by current system if nothing else)
// at that point the transforms can just be handed whatever the current universe time is when they come into view
// this will require complicated handling for things that have routes
QList<Transform*> transforms;

// for now hard code the solar system because to heck with it i tell you
SolarSystemType sol = SolarSystemType(695700000000, 1988500000000000);

void universe_init(void)
{
    // for now hard code the solar system because to heck with it i tell you
    sol.root.color = QColor(226, 223, 24); // nice sun color

    static CelestialType mercury = CelestialType(2439700000, 330110000, 57909050000000, &sol.root);

    static CelestialType venus = CelestialType(6051800000, 4867500000U, 108208000000000, &sol.root);
    venus.color = QColor(239, 119, 14);

    static CelestialType earth = CelestialType(6378100000, 5972200000U, 149598023000000, &sol.root);
    earth.color = QColor(19, 33, 219);
    //moon
    static CelestialType moon = CelestialType(1737400000, 73420000U, 384399000000, &earth);

    //mars
    static CelestialType mars = CelestialType(3389500000, 641710000U, 227939200000000, &sol.root);
    mars.color = QColor(242, 76, 26);
    //phobos/deimos
    static CelestialType phobos = CelestialType(11266700, 11U, 9376000000, &mars);
    static CelestialType deimos = CelestialType(6200000, 1U, 23463200000, &mars);

    static CelestialType jupiter = CelestialType(69911000000, 1898200000000, 778570000000000, &sol.root);
    jupiter.color = QColor(249, 194, 164);

    static CelestialType saturn = CelestialType(58232000000, 568340000000, 1433530000000000, &sol.root);
    saturn.color = QColor(198,160,110);

    static CelestialType uranus = CelestialType(25559000000, 86810000000, 2870972000000000, &sol.root);
    uranus.color = QColor(141,161,170);

    static CelestialType neptune = CelestialType(24622000000, 102413000000, 4500000000000000, &sol.root);
    neptune.color = QColor(98,122,169);

    static CelestialType pluto = CelestialType(1188300000, 13030000, 5906380000000000, &sol.root);

    static FleetType testfleet1 = FleetType(&earth, 400000000 + earth.radius);
    static FleetType testfleet2 = FleetType(&earth, 400000000 + earth.radius);
    static FleetType testfleet3 = FleetType(&earth, 400000000 + earth.radius);
}

// delta t in milliseconds
void universe_update(long delta_t)
{
    if (universe_paused)
        return;

    static long remainder_milliseconds = 0;

    if (universe_time_warp < 0)
    {
        delta_t += remainder_milliseconds;
        long mask = (1 << -1*universe_time_warp) - 1;
        remainder_milliseconds = delta_t & mask;
        universe_time += (delta_t >> -1*universe_time_warp);
    }
    else
    {
        // handle case where we transition back into positive warp (bit wasteful but meh its just integer addition)
        universe_time += (delta_t << universe_time_warp) + remainder_milliseconds;
        remainder_milliseconds = 0; // only need to do this once really
    }

    foreach (Transform *t, transforms)
    {
        t->UpdatePosition();
    }

    //TODO: ?
}
