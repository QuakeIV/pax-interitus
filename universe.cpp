#include "universe.h"
#include "transform.h"
#include "celestialtype.h"
#include "fleettype.h"
#include "solarsystemtype.h"
#include "spacecraft.h"

// TODO: temporary
Empire player_empire;

// master list of extant dialectric materials
QList<Dialectric> dialectric_materials;

bool universe_paused = false;
int64_t universe_time_warp = 0; //this is a power applied to 2
// TODO: we may be obliged to go over into microseconds or something to get better definition on fast-moving objects
// we have plenty of resolution left, at present we could model time out to +/-146 million years from t=0 (however admittedly it would be fun to have planet formation levels of time scale)
// TODO: negative time is extremely not-functional
int64_t universe_time;

// universe time for next event (drives update scheduling)
// TODO: we really need to be careful with how this gets set
uint64_t universe_next_event = LLONG_MAX;

//TODO: it might be better to only update the transforms we are currently looking at (cull by current system if nothing else)
// at that point the transforms can just be handed whatever the current universe time is when they come into view
// this will require complicated handling for things that have routes
QList<Transform*> transforms;

// track all extant solar systems
QList<SolarSystemType*> systems;

// track all spacecraft in existence
QList<Spacecraft*> spacecraft;

// put the fleet type auto increment in the universe because it has to live in some compile unit
uint64_t FleetType::fleet_id = 0;

void universe_init(void)
{
    // TODO: temporary
    // add some dialectric materials
    Dialectric d;
    d.permittivity = 20;
    d.strength = 20000;
    d.name = "wumbo";
    dialectric_materials.append(d);
    d.permittivity = 10;
    d.strength = 40000;
    d.name = "mumbo";
    dialectric_materials.append(d);
    d.permittivity = 30;
    d.strength = 30000;
    d.name = "jumbo";
    dialectric_materials.append(d);

    // for now hard code the solar system because to heck with it i tell you
    static SolarSystemType sol = SolarSystemType(695700000000, 1988500000000000);
    sol.root.color = QColor(226, 223, 24); // nice sun color
    sol.root.name = "Sol";
    sol.name = "Sol";

    static CelestialType mercury = CelestialType(2439700000, 330110000, 57909050000000, &sol.root);
    mercury.name = "Mercury";

    static CelestialType venus = CelestialType(6051800000, 4867500000U, 108208000000000, &sol.root);
    venus.name = "Venus";
    venus.color = QColor(239, 119, 14);

    static CelestialType earth = CelestialType(6378100000, 5972200000U, 149598023000000, &sol.root);
    earth.color = QColor(19, 33, 219);
    earth.name = "Earth";
    //moon
    static CelestialType moon = CelestialType(1737400000, 73420000U, 384399000000, &earth);
    moon.name = "Luna";

    //mars
    static CelestialType mars = CelestialType(3389500000, 641710000U, 227939200000000, &sol.root);
    mars.name = "Mars";
    mars.color = QColor(242, 76, 26);
    //phobos/deimos
    static CelestialType phobos = CelestialType(11266700, 11U, 9376000000, &mars);
    phobos.name = "Phobos";
    static CelestialType deimos = CelestialType(6200000, 1U, 23463200000, &mars);
    deimos.name = "Deimos";

    static CelestialType jupiter = CelestialType(69911000000, 1898200000000, 778570000000000, &sol.root);
    jupiter.name = "Jupiter";
    jupiter.color = QColor(249, 194, 164);
    static CelestialType ganymede = CelestialType(2634100000, 148190000, 1070412000000, &jupiter); //also sorted by mass
    ganymede.name = "Ganymede";
    static CelestialType callisto = CelestialType(2410300000, 107590000, 1882709000000, &jupiter);
    callisto.name = "Callisto";
    static CelestialType io       = CelestialType(1821600000, 89319000,  421700000000, &jupiter);
    io.name = "Io";
    static CelestialType europa   = CelestialType(1560800000, 47999000,  671034000000, &jupiter);
    europa.name = "Europa";
    static CelestialType humalia  = CelestialType(139600000,  4200,      11497400000000, &jupiter);
    humalia.name = "Humalia";
    static CelestialType amalthea = CelestialType(167000000,  2080,      181366000000, &jupiter);
    amalthea.name = "Amalthea";
    static CelestialType elara    = CelestialType(79900000,   870,       11671600000000, &jupiter);
    elara.name = "Elara";
    static CelestialType thebe    = CelestialType(98600000,   430,       222452000000, &jupiter);
    thebe.name = "Thebe";
    static CelestialType pasiphae = CelestialType(57800000,   300,       23119300000000, &jupiter);
    pasiphae.name = "Pasiphae";
    static CelestialType carme    = CelestialType(46700000,   130,       22579900000000, &jupiter);
    carme.name = "Carme";
    static CelestialType sinope   = CelestialType(35000000,   75,        24864100000000, &jupiter);
    sinope.name = "Sinope";
    static CelestialType lysithea = CelestialType(42200000,   63,        11628300000000, &jupiter);
    lysithea.name = "Lysithea";
    static CelestialType metis    = CelestialType(43000000,   36,        128852000000, &jupiter);
    metis.name = "Metis";
    static CelestialType ananke   = CelestialType(29100000,   30,        21042500000000, &jupiter);
    ananke.name = "Ananke";
    static CelestialType leda     = CelestialType(21500000,   6,         11196000000000, &jupiter);
    leda.name = "Leda";
    static CelestialType adrastea = CelestialType(16400000,   2,         129000000000, &jupiter);
    adrastea.name = "Adrastea";

    static CelestialType saturn = CelestialType(58232000000, 568340000000, 1433530000000000, &sol.root);
    saturn.name = "Saturn";
    saturn.color = QColor(198,160,110);
    static CelestialType titan      = CelestialType(2574730000, 134520000, 1221930000000, &saturn);
    titan.name = "Titan";
    static CelestialType rhea       = CelestialType(763800000,  2306518,   527108000000, &saturn);
    rhea.name = "Rhea";
    static CelestialType iapetus    = CelestialType(734300000,  1805635,   3560820000000, &saturn);
    iapetus.name = "Iapetus";
    static CelestialType dione      = CelestialType(561400000,  1095452,   377396000000, &saturn);
    dione.name = "Dione";
    static CelestialType tethys     = CelestialType(531100000,  617449,    294619000000, &saturn);
    tethys.name = "Tethys";
    static CelestialType enceladus  = CelestialType(252100000,  108022,    237950000000, &saturn);
    enceladus.name = "Enceladus";
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
    uranus.name = "Uranus";
    uranus.color = QColor(141,161,170);
    static CelestialType titania   = CelestialType(788400000, 3400000, 435910000000, &uranus);
    static CelestialType oberon    = CelestialType(776400000, 3070000, 583520000000, &uranus);
    static CelestialType umbriel   = CelestialType(584700000, 1275000, 266300000000, &uranus);
    static CelestialType ariel     = CelestialType(578900000, 1251000, 191020000000, &uranus);
    static CelestialType miranda   = CelestialType(235800000, 64000,   129390000000, &uranus);
    static CelestialType puck      = CelestialType(81000000,  2900,    86010000000, &uranus);
    static CelestialType sycorax   = CelestialType(78500000,  2300,    12179400000000, &uranus);
    static CelestialType portia    = CelestialType(67500000,  1700,    69090000000, &uranus);
    static CelestialType juliet    = CelestialType(47000000,  560,     64350000000, &uranus);
    static CelestialType belinda   = CelestialType(45000000,  490,     75260000000, &uranus);
    static CelestialType cressida  = CelestialType(40000000,  340,     61780000000, &uranus);
    static CelestialType rosalind  = CelestialType(36000000,  250,     69940000000, &uranus);
    static CelestialType caliban   = CelestialType(21000000,  250,     7231100000000, &uranus);
    static CelestialType desdemona = CelestialType(32000000,  180,     62680000000, &uranus);
    static CelestialType bianca    = CelestialType(25500000,  92,      59170000000, &uranus);
    static CelestialType prospero  = CelestialType(25000000,  85,      16276800000000, &uranus);
    static CelestialType setebos   = CelestialType(24000000,  75,      17420400000000, &uranus);
    static CelestialType ophelia   = CelestialType(21500000,  53,      53790000000, &uranus);
    static CelestialType cordelia  = CelestialType(20000000,  44,      49770000000, &uranus);
    static CelestialType stephano  = CelestialType(16000000,  22,      8007400000000, &uranus);
    static CelestialType perdita   = CelestialType(15000000,  18,      76400000000, &uranus);
    static CelestialType mab       = CelestialType(12500000,  10,      97700000000, &uranus);
    static CelestialType francisco = CelestialType(11000000,  7,       4282900000000, &uranus);
    static CelestialType margaret  = CelestialType(10000000,  5,       14146700000000, &uranus);
    static CelestialType ferdinand = CelestialType(10000000,  5,       20430000000000, &uranus);
    static CelestialType trinculo  = CelestialType(9000000,   4,       8505200000000, &uranus);
    static CelestialType cupid     = CelestialType(9000000,   4,       74800000000, &uranus);

    static CelestialType neptune = CelestialType(24622000000, 102413000000, 4500000000000000, &sol.root);
    neptune.name = "Neptune";
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
    pluto.name = "Pluto";
    static CelestialType charon = CelestialType(606000000, 1587000, 17536000000, &pluto); // pluto moons by mass (because eh)
    static CelestialType nix = CelestialType(45000000, 50, 48694000000, &pluto);
    static CelestialType hydra = CelestialType(45000000, 50, 64738000000, &pluto);
    static CelestialType kerberos = CelestialType(15000000, 16, 57783000000, &pluto);
    static CelestialType styx = CelestialType(12000000, 8, 42656000000, &pluto);

    static FleetType testfleet1 = FleetType(&earth, 400000000 + earth.radius);
    static FleetType testfleet2 = FleetType(&earth, 400000000 + earth.radius);
    static FleetType testfleet3 = FleetType(&earth, 400000000 + earth.radius);

    static Spacecraft testcraft = Spacecraft();
    testfleet1.ships.append(&testcraft);
}

// delta t in milliseconds
void universe_update(int64_t delta_t)
{
    if (universe_paused)
        return;

    static int64_t remainder_microseconds = 0;

    if (universe_time_warp < 0)
    {
        delta_t += remainder_microseconds;
        int64_t mask = (1 << -1*universe_time_warp) - 1;
        remainder_microseconds = delta_t & mask;
        delta_t = (delta_t >> -1*universe_time_warp);
    }
    else
    {
        // handle case where we transition back into positive warp (bit wasteful but meh its just integer addition)
        delta_t = (delta_t << universe_time_warp) + remainder_microseconds;
        remainder_microseconds = 0; // only need to do this once really
    }

    if (universe_next_event < universe_time)
        qDebug() << "ERROR: universe next event somehow set into the past";
    if ((delta_t + universe_time) > universe_next_event)
        universe_time = universe_next_event;
    else
        universe_time += delta_t;
    // reset next event to the end of time, and then let the following update functions set 'next event' accordingly if they indeed have anything to say on the matter
    universe_next_event = LLONG_MAX;

    // TODO: respect orbit heirarchy instead of just blindly traversing list
    foreach (Transform *t, transforms)
    {
        t->update_position();
    }

    foreach (Spacecraft *s, spacecraft)
    {
        s->update(delta_t);
    }

    //TODO: ?
}
