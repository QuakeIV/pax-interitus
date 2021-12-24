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
    static CelestialType titan      = CelestialType(2574730000, 134520000, 1221930000000, &saturn);
    static CelestialType rhea       = CelestialType(763800000,  2306518,   527108000000, &saturn);
    static CelestialType iapetus    = CelestialType(734300000,  1805635,   3560820000000, &saturn);
    static CelestialType dione      = CelestialType(561400000,  1095452,   377396000000, &saturn);
    static CelestialType tethys     = CelestialType(531100000,  617449,    294619000000, &saturn);
    static CelestialType enceladus  = CelestialType(252100000,  108022,    237950000000, &saturn);
    static CelestialType mimas      = CelestialType(198200000,  37493,     185404000000, &saturn);
    static CelestialType phoebe     = CelestialType(106500000,  8292,      12905900000000, &saturn);
    static CelestialType hyperion   = CelestialType(135000000,  5619,      1481010000000, &saturn);
    static CelestialType janus      = CelestialType(89500000,   1898,      151472000000, &saturn);
    static CelestialType epimetheus = CelestialType(58100000,   527,       151422000000, &saturn);
    static CelestialType prometheus = CelestialType(43100000,   160,       139380000000, &saturn);
    static CelestialType pandora    = CelestialType(40700000,   137,       141720000000, &saturn);
    static CelestialType siamaq     = CelestialType(19650000,   44,        17937000000000, &saturn);
    static CelestialType helene     = CelestialType(17600000,   25,        377396000000, &saturn);
    static CelestialType albiorix   = CelestialType(14300000,   22,        16222700000000, &saturn);
    static CelestialType telesto    = CelestialType(12400000,   9,         294619000000, &saturn);
    static CelestialType paaliaq    = CelestialType(12500000,   7,         15012800000000, &saturn);
    static CelestialType atlas      = CelestialType(15100000,   7,         137670000000, &saturn);
    static CelestialType calypso    = CelestialType(10700000,   6,         294619000000, &saturn);
    static CelestialType pan        = CelestialType(14100000,   5,         133584000000, &saturn);
    static CelestialType ymir       = CelestialType(9500000,    4,         22841900000000, &saturn);
    static CelestialType kivluq     = CelestialType(8500000,    3,         11307500000000, &saturn);
    static CelestialType tarvos     = CelestialType(7500000,    2,         18243800000000, &saturn);
    static CelestialType ijiraq     = CelestialType(8500000,    1,         11348500000000, &saturn);

    static CelestialType uranus = CelestialType(25559000000, 86810000000, 2870972000000000, &sol.root);
    uranus.color = QColor(141,161,170);

    static CelestialType neptune = CelestialType(24622000000, 102413000000, 4500000000000000, &sol.root);
    neptune.color = QColor(98,122,169);
    // TODO: retrograde orbits
    static CelestialType triton    = CelestialType(1352600000, 21390000, 354759000000, &neptune); //also mass order
    static CelestialType proteus   = CelestialType(210000000,  44000,    117646000000, &neptune);
    static CelestialType nereid    = CelestialType(178500000,  27000,    5513800000000, &neptune);
    static CelestialType larissa   = CelestialType(97000000,   4200,     73548000000, &neptune);
    static CelestialType despina   = CelestialType(78000000,   2200,     52526000000, &neptune);
    static CelestialType galatea   = CelestialType(87400000,   2120,     61953000000, &neptune);
    static CelestialType thalassa  = CelestialType(40700000,   350,      50074000000, &neptune);
    static CelestialType naiad     = CelestialType(30200000,   190,      48224000000, &neptune);
    static CelestialType halimede  = CelestialType(31000000,   160,      16681000000000, &neptune);
    static CelestialType neso      = CelestialType(30000000,   150,      50258000000000, &neptune);
    static CelestialType sao       = CelestialType(22000000,   60,       22619000000000, &neptune);
    static CelestialType laomedeia = CelestialType(21000000,   50,       22613000000000, &neptune);
    static CelestialType psamthe   = CelestialType(20000000,   40,       46705000000000, &neptune);
    static CelestialType hippocamp = CelestialType(17400000,   30,       105283000000, &neptune);

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
