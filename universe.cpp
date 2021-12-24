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
    static CelestialType ganymede = CelestialType(2634100000, 148190000, 1070412000000, &jupiter); //also sorted by mass
    static CelestialType callisto = CelestialType(2410300000, 107590000, 1882709000000, &jupiter);
    static CelestialType io       = CelestialType(1821600000, 89319000,  421700000000, &jupiter);
    static CelestialType europa   = CelestialType(1560800000, 47999000,  671034000000, &jupiter);
    static CelestialType humalia  = CelestialType(139600000,  4200,      11497400000000, &jupiter);
    static CelestialType amalthea = CelestialType(167000000,  2080,      181366000000, &jupiter);
    static CelestialType elara    = CelestialType(79900000,   870,       11671600000000, &jupiter);
    static CelestialType thebe    = CelestialType(98600000,   430,       222452000000, &jupiter);
    static CelestialType pasiphae = CelestialType(57800000,   300,       23119300000000, &jupiter);
    static CelestialType carme    = CelestialType(46700000,   130,       22579900000000, &jupiter);
    static CelestialType sinope   = CelestialType(35000000,   75,        24864100000000, &jupiter);
    static CelestialType lysithea = CelestialType(42200000,   63,        11628300000000, &jupiter);
    static CelestialType metis    = CelestialType(43000000,   36,        128852000000, &jupiter);
    static CelestialType ananke   = CelestialType(29100000,   30,        21042500000000, &jupiter);
    static CelestialType leda     = CelestialType(21500000,   6,         11196000000000, &jupiter);
    static CelestialType adrastea = CelestialType(16400000,   2,         129000000000, &jupiter);

    static CelestialType saturn = CelestialType(58232000000, 568340000000, 1433530000000000, &sol.root);
    saturn.color = QColor(198,160,110);

    static CelestialType uranus = CelestialType(25559000000, 86810000000, 2870972000000000, &sol.root);
    uranus.color = QColor(141,161,170);

    static CelestialType neptune = CelestialType(24622000000, 102413000000, 4500000000000000, &sol.root);
    neptune.color = QColor(98,122,169);

    //TODO: pluto is actually significantly barycentric with charon
    static CelestialType pluto = CelestialType(1188300000, 13030000, 5906380000000000, &sol.root);
    static CelestialType charon = CelestialType(606000000, 1587000, 17536000000, &pluto); // pluto moons by mass (because eh)
    static CelestialType nix = CelestialType(45000000, 50, 48694000000, &pluto);
    static CelestialType hydra = CelestialType(45000000, 50, 64738000000, &pluto);
    static CelestialType kerberos = CelestialType(15000000, 16, 57783000000, &pluto);
    static CelestialType styx = CelestialType(12000000, 8, 42656000000, &pluto);

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
