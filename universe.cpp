#include "universe.h"
#include "transform.h"
#include "celestialtype.h"

bool universe_paused = false;
long universe_time_warp = 0; //this is a power applied to 2
long universe_time;

QList<Transform*> transforms;

CelestialType *sol;

void universe_init(void)
{
    // for now hard code the solar system because to heck with it i tell you
    static CelestialType root = CelestialType(695700000000, 1988500000000000);
    root.color = QColor(226, 223, 24); // nice sun color
    sol = &root;

    static CelestialType mercury = CelestialType(2439700000, 330110000, 57909050000000, sol);

    static CelestialType venus = CelestialType(6051800000, 4867500000U, 108208000000000, sol);
    venus.color = QColor(239, 119, 14);

    static CelestialType earth = CelestialType(6378100000, 5972200000U, 149598023000000, sol);
    earth.color = QColor(19, 33, 219);
    //moon
    static CelestialType moon = CelestialType(1737400000, 73420000U, 384399000000, &earth);

    //mars
    static CelestialType mars = CelestialType(3389500000, 641710000U, 227939200000000, sol);
    mars.color = QColor(242, 76, 26);
    //phobos/deimos
    static CelestialType phobos = CelestialType(11266700, 11U, 9376000000, &mars);
    static CelestialType deimos = CelestialType(6200000, 1U, 23463200000, &mars);
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
