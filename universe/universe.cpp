#include "universe.h"
#include "transform.h"
#include "celestial.h"
#include "spacecraft/taskgroup.h"
#include "solarsystemtype.h"
#include "spacecraft/spacecraft.h"
#include "units.h"
#include <thread>
#include <chrono>

// TODO: temporary
Empire player_empire;

// TODO: temporary
QApplication *qapp;

// master list of extant dialectric materials
QList<Insulator*> insulator_materials;

bool universe_paused = false;
int64_t universe_time_warp = 0; //this is a power applied to 2
// TODO: we may be obliged to go over into microseconds or something to get better definition on fast-moving objects
// we have plenty of resolution left, at present we could model time out to +/-146 million years from t=0 (however admittedly it would be fun to have planet formation levels of time scale)
// TODO: negative time is extremely not-functional
int64_t universe_time;

// universe time for next event (drives update scheduling)
// TODO: we really need to be careful with how this gets set
int64_t universe_next_event = INT64_MAX;

//TODO: it might be better to only update the transforms we are currently looking at (cull by current system if nothing else)
// at that point the transforms can just be handed whatever the current universe time is when they come into view
// this will require complicated handling for things that have routes
QList<Transform*> transforms;

// track all extant solar systems
QList<SolarSystemType*> systems;

// track all spacecraft in existence
QList<Spacecraft*> spacecraft;
// TODO: this should most definitively be stored as part of an empire
QList<SpacecraftDesign*> spacecraft_designs;

// put the fleet type auto increment in the universe because it has to live in some compile unit
uint64_t Taskgroup::group_id = 0;

// this will exist to separate the timing logic from the actual universe update logic,
// as this will probably change periodically, for instance if we escape C++
static std::thread *universe_updater;
const int frame_time_us = 16667;
void universe_timing_loop(void)
{
    while (true)
    {
        std::chrono::time_point start = std::chrono::system_clock::now();
        universe_update(MICROSECONDS_TO_TIME(frame_time_us));
        std::chrono::time_point end = std::chrono::system_clock::now();
        std::this_thread::sleep_for(std::chrono::microseconds(frame_time_us) - (end-start));

        // lazy diagnosis of frame overrun
        // TODO: (wow this chrono library seems horrible and vastly overweight)
        // not that vital to gun for accurate time, minding (stolen from mainwindow.cpp which now longer handles this)
        //end = std::chrono::system_clock::now();
        //printf("woden %ld\n", std::chrono::duration_cast<std::chrono::microseconds>(end-start).count());
    }
}

void universe_init(void)
{
    // TODO: temporary
    // add some dialectric materials
    Insulator *i;
    // now featuring wikipedia capacitor materials
    // (most optimistic figures)
    // TODO: it might be better to go the aurora route and just have 'conventional' followed by mainly fanciful materials
    i = new Insulator();
    i->permittivity = RELATIVE_DIALECTRIC_TO_ABSOLUTE(40);
    i->strength = VOLT_UM_TO_VOLT_M(100);
    i->name = "Ceramic Class 1";
    insulator_materials.append(i);
    i = new Insulator();
    i->permittivity = RELATIVE_DIALECTRIC_TO_ABSOLUTE(14000);
    i->strength = VOLT_UM_TO_VOLT_M(35);
    i->name = "Ceramic Class 2";
    insulator_materials.append(i);
    i = new Insulator();
    i->permittivity = RELATIVE_DIALECTRIC_TO_ABSOLUTE(3.3);
    i->strength = VOLT_UM_TO_VOLT_M(580);
    i->name = "Film (PET)";
    insulator_materials.append(i);
    i = new Insulator();
    i->permittivity = RELATIVE_DIALECTRIC_TO_ABSOLUTE(9.6);
    i->strength = VOLT_UM_TO_VOLT_M(710);
    i->name = "Aluminum electrolytic";
    insulator_materials.append(i);
    i = new Insulator();
    i->permittivity = RELATIVE_DIALECTRIC_TO_ABSOLUTE(26);
    i->strength = VOLT_UM_TO_VOLT_M(625);
    i->name = "Tantalum electrolytic";
    insulator_materials.append(i);
    i = new Insulator();
    i->permittivity = RELATIVE_DIALECTRIC_TO_ABSOLUTE(42);
    i->strength = VOLT_UM_TO_VOLT_M(455);
    i->name = "Niobium electrolytic";
    insulator_materials.append(i);
    i = new Insulator();
    i->permittivity = RELATIVE_DIALECTRIC_TO_ABSOLUTE(10);
    i->strength = VOLT_UM_TO_VOLT_M(450);
    i->name = "Glass";
    insulator_materials.append(i);
    i = new Insulator();
    i->permittivity = RELATIVE_DIALECTRIC_TO_ABSOLUTE(8);
    i->strength = VOLT_UM_TO_VOLT_M(118);
    i->name = "Mica";
    insulator_materials.append(i);

    // regular insulators (dunno the permittivity but presumably poor)
    i = new Insulator();
    i->permittivity = RELATIVE_DIALECTRIC_TO_ABSOLUTE(10);
    i->strength = VOLT_UM_TO_VOLT_M(2000);
    i->name = "Diamond";
    insulator_materials.append(i);
    i = new Insulator();
    i->permittivity = 0;// TODO: placeholder for not-known permittivity
    i->strength = VOLT_UM_TO_VOLT_M(670);
    i->name = "Fused Silica";
    insulator_materials.append(i);
    i = new Insulator();
    i->permittivity = 0;
    i->strength = VOLT_UM_TO_VOLT_M(160);
    i->name = "Polyethylene";
    insulator_materials.append(i);
    i = new Insulator();
    i->permittivity = 0;
    i->strength = VOLT_UM_TO_VOLT_M(170);
    i->name = "Teflon Film";
    insulator_materials.append(i);

    // for now hard code the solar system because to heck with it i tell you
    static SolarSystemType sol = SolarSystemType(695700000000, 1988500000000000);
    sol.root.color = QColor(226, 223, 24); // nice sun color
    sol.root.name = "Sol";
    sol.name = "Sol";

    static Celestial mercury = Celestial(2439700000, 330110000, 57909050000000, &sol.root);
    mercury.name = "Mercury";

    static Celestial venus = Celestial(6051800000, 4867500000U, 108208000000000, &sol.root);
    venus.name = "Venus";
    venus.color = QColor(239, 119, 14);

    static Celestial earth = Celestial(6378100000, 5972200000U, 149598023000000, &sol.root);
    earth.color = QColor(19, 33, 219);
    earth.name = "Earth";
    //moon
    static Celestial moon = Celestial(1737400000, 73420000U, 384399000000, &earth);
    moon.name = "Luna";

    //mars
    static Celestial mars = Celestial(3389500000, 641710000U, 227939200000000, &sol.root);
    mars.name = "Mars";
    mars.color = QColor(242, 76, 26);
    //phobos/deimos
    static Celestial phobos = Celestial(11266700, 11U, 9376000000, &mars);
    phobos.name = "Phobos";
    static Celestial deimos = Celestial(6200000, 1U, 23463200000, &mars);
    deimos.name = "Deimos";

    static Celestial jupiter = Celestial(69911000000, 1898200000000, 778570000000000, &sol.root);
    jupiter.name = "Jupiter";
    jupiter.color = QColor(249, 194, 164);
    static Celestial ganymede = Celestial(2634100000, 148190000, 1070412000000, &jupiter); //also sorted by mass
    ganymede.name = "Ganymede";
    static Celestial callisto = Celestial(2410300000, 107590000, 1882709000000, &jupiter);
    callisto.name = "Callisto";
    static Celestial io       = Celestial(1821600000, 89319000,  421700000000, &jupiter);
    io.name = "Io";
    static Celestial europa   = Celestial(1560800000, 47999000,  671034000000, &jupiter);
    europa.name = "Europa";
    static Celestial humalia  = Celestial(139600000,  4200,      11497400000000, &jupiter);
    humalia.name = "Humalia";
    static Celestial amalthea = Celestial(167000000,  2080,      181366000000, &jupiter);
    amalthea.name = "Amalthea";
    static Celestial elara    = Celestial(79900000,   870,       11671600000000, &jupiter);
    elara.name = "Elara";
    static Celestial thebe    = Celestial(98600000,   430,       222452000000, &jupiter);
    thebe.name = "Thebe";
    static Celestial pasiphae = Celestial(57800000,   300,       23119300000000, &jupiter);
    pasiphae.name = "Pasiphae";
    static Celestial carme    = Celestial(46700000,   130,       22579900000000, &jupiter);
    carme.name = "Carme";
    static Celestial sinope   = Celestial(35000000,   75,        24864100000000, &jupiter);
    sinope.name = "Sinope";
    static Celestial lysithea = Celestial(42200000,   63,        11628300000000, &jupiter);
    lysithea.name = "Lysithea";
    static Celestial metis    = Celestial(43000000,   36,        128852000000, &jupiter);
    metis.name = "Metis";
    static Celestial ananke   = Celestial(29100000,   30,        21042500000000, &jupiter);
    ananke.name = "Ananke";
    static Celestial leda     = Celestial(21500000,   6,         11196000000000, &jupiter);
    leda.name = "Leda";
    static Celestial adrastea = Celestial(16400000,   2,         129000000000, &jupiter);
    adrastea.name = "Adrastea";

    static Celestial saturn = Celestial(58232000000, 568340000000, 1433530000000000, &sol.root);
    saturn.name = "Saturn";
    saturn.color = QColor(198,160,110);
    static Celestial titan      = Celestial(2574730000, 134520000, 1221930000000, &saturn);
    titan.name = "Titan";
    static Celestial rhea       = Celestial(763800000,  2306518,   527108000000, &saturn);
    rhea.name = "Rhea";
    static Celestial iapetus    = Celestial(734300000,  1805635,   3560820000000, &saturn);
    iapetus.name = "Iapetus";
    static Celestial dione      = Celestial(561400000,  1095452,   377396000000, &saturn);
    dione.name = "Dione";
    static Celestial tethys     = Celestial(531100000,  617449,    294619000000, &saturn);
    tethys.name = "Tethys";
    static Celestial enceladus  = Celestial(252100000,  108022,    237950000000, &saturn);
    enceladus.name = "Enceladus";
    static Celestial mimas      = Celestial(198200000,  37493,     185404000000, &saturn);
    static Celestial phoebe     = Celestial(106500000,  8292,      12905900000000, &saturn);
    static Celestial hyperion   = Celestial(135000000,  5619,      1481010000000, &saturn);
    static Celestial janus      = Celestial(89500000,   1898,      151472000000, &saturn);
    static Celestial epimetheus = Celestial(58100000,   527,       151422000000, &saturn);
    static Celestial prometheus = Celestial(43100000,   160,       139380000000, &saturn);
    static Celestial pandora    = Celestial(40700000,   137,       141720000000, &saturn);
    static Celestial siamaq     = Celestial(19650000,   44,        17937000000000, &saturn);
    static Celestial helene     = Celestial(17600000,   25,        377396000000, &saturn);
    static Celestial albiorix   = Celestial(14300000,   22,        16222700000000, &saturn);
    static Celestial telesto    = Celestial(12400000,   9,         294619000000, &saturn);
    static Celestial paaliaq    = Celestial(12500000,   7,         15012800000000, &saturn);
    static Celestial atlas      = Celestial(15100000,   7,         137670000000, &saturn);
    static Celestial calypso    = Celestial(10700000,   6,         294619000000, &saturn);
    static Celestial pan        = Celestial(14100000,   5,         133584000000, &saturn);
    static Celestial ymir       = Celestial(9500000,    4,         22841900000000, &saturn);
    static Celestial kivluq     = Celestial(8500000,    3,         11307500000000, &saturn);
    static Celestial tarvos     = Celestial(7500000,    2,         18243800000000, &saturn);
    static Celestial ijiraq     = Celestial(8500000,    1,         11348500000000, &saturn);

    static Celestial uranus = Celestial(25559000000, 86810000000, 2870972000000000, &sol.root);
    uranus.name = "Uranus";
    uranus.color = QColor(141,161,170);
    static Celestial titania   = Celestial(788400000, 3400000, 435910000000, &uranus);
    static Celestial oberon    = Celestial(776400000, 3070000, 583520000000, &uranus);
    static Celestial umbriel   = Celestial(584700000, 1275000, 266300000000, &uranus);
    static Celestial ariel     = Celestial(578900000, 1251000, 191020000000, &uranus);
    static Celestial miranda   = Celestial(235800000, 64000,   129390000000, &uranus);
    static Celestial puck      = Celestial(81000000,  2900,    86010000000, &uranus);
    static Celestial sycorax   = Celestial(78500000,  2300,    12179400000000, &uranus);
    sycorax.name = "Sycorax";
    static Celestial portia    = Celestial(67500000,  1700,    69090000000, &uranus);
    static Celestial juliet    = Celestial(47000000,  560,     64350000000, &uranus);
    static Celestial belinda   = Celestial(45000000,  490,     75260000000, &uranus);
    static Celestial cressida  = Celestial(40000000,  340,     61780000000, &uranus);
    static Celestial rosalind  = Celestial(36000000,  250,     69940000000, &uranus);
    static Celestial caliban   = Celestial(21000000,  250,     7231100000000, &uranus);
    static Celestial desdemona = Celestial(32000000,  180,     62680000000, &uranus);
    static Celestial bianca    = Celestial(25500000,  92,      59170000000, &uranus);
    static Celestial prospero  = Celestial(25000000,  85,      16276800000000, &uranus);
    static Celestial setebos   = Celestial(24000000,  75,      17420400000000, &uranus);
    setebos.name = "Setebos";
    static Celestial ophelia   = Celestial(21500000,  53,      53790000000, &uranus);
    static Celestial cordelia  = Celestial(20000000,  44,      49770000000, &uranus);
    static Celestial stephano  = Celestial(16000000,  22,      8007400000000, &uranus);
    static Celestial perdita   = Celestial(15000000,  18,      76400000000, &uranus);
    static Celestial mab       = Celestial(12500000,  10,      97700000000, &uranus);
    static Celestial francisco = Celestial(11000000,  7,       4282900000000, &uranus);
    static Celestial margaret  = Celestial(10000000,  5,       14146700000000, &uranus);
    static Celestial ferdinand = Celestial(10000000,  5,       20430000000000, &uranus);
    static Celestial trinculo  = Celestial(9000000,   4,       8505200000000, &uranus);
    static Celestial cupid     = Celestial(9000000,   4,       74800000000, &uranus);
    cupid.name = "Cupid";

    static Celestial neptune = Celestial(24622000000, 102413000000, 4500000000000000, &sol.root);
    neptune.name = "Neptune";
    neptune.color = QColor(98,122,169);
    // TODO: retrograde orbits
    static Celestial triton    = Celestial(1352600000, 21390000, 354759000000, &neptune); //also mass order
    static Celestial proteus   = Celestial(210000000,  44000,    117646000000, &neptune);
    static Celestial nereid    = Celestial(178500000,  27000,    5513800000000, &neptune);
    static Celestial larissa   = Celestial(97000000,   4200,     73548000000, &neptune);
    static Celestial despina   = Celestial(78000000,   2200,     52526000000, &neptune);
    static Celestial galatea   = Celestial(87400000,   2120,     61953000000, &neptune);
    static Celestial thalassa  = Celestial(40700000,   350,      50074000000, &neptune);
    static Celestial naiad     = Celestial(30200000,   190,      48224000000, &neptune);
    static Celestial halimede  = Celestial(31000000,   160,      16681000000000, &neptune);
    static Celestial neso      = Celestial(30000000,   150,      50258000000000, &neptune);
    static Celestial sao       = Celestial(22000000,   60,       22619000000000, &neptune);
    sao.name = "Sao";
    static Celestial laomedeia = Celestial(21000000,   50,       22613000000000, &neptune);
    static Celestial psamthe   = Celestial(20000000,   40,       46705000000000, &neptune);
    psamthe.name = "Psamthe";
    static Celestial hippocamp = Celestial(17400000,   30,       105283000000, &neptune);

    //TODO: pluto is actually significantly barycentric with charon
    static Celestial pluto = Celestial(1188300000, 13030000, 5906380000000000, &sol.root);
    pluto.name = "Pluto";
    static Celestial charon = Celestial(606000000, 1587000, 17536000000, &pluto); // pluto moons by mass (because eh)
    charon.name = "Charon";
    static Celestial nix = Celestial(45000000, 50, 48694000000, &pluto);
    nix.name = "Nix";
    static Celestial hydra = Celestial(45000000, 50, 64738000000, &pluto);
    hydra.name = "Hydra";
    static Celestial kerberos = Celestial(15000000, 16, 57783000000, &pluto);
    kerberos.name = "Kerberos";
    static Celestial styx = Celestial(12000000, 8, 42656000000, &pluto);
    styx.name = "Styx";

    static Spacecraft testcraft1 = Spacecraft();
    testcraft1.name = "SS Test 1";
    static Transform epic_static_position = Transform();
    epic_static_position.position.x = -105684699015963;
    epic_static_position.position.y = -27305789478705;
    testcraft1.trajectory = &epic_static_position;
    sol.spacecraft.append(&testcraft1);
    static Spacecraft testcraft2 = Spacecraft();
    static OrbitType t1 = OrbitType(&earth, earth.radius + 400000000);
    testcraft2.trajectory = &t1;
    testcraft2.name = "SS Test 2";
    sol.spacecraft.append(&testcraft2);
    static Spacecraft testcraft3 = Spacecraft();
    static OrbitType t2 = OrbitType(&earth, earth.radius + 400000000);
    testcraft3.trajectory = &t2;
    testcraft3.name = "SS Test 3";
    sol.spacecraft.append(&testcraft3);
    static Spacecraft testcraft4 = Spacecraft();
    static OrbitType t3 = OrbitType(&earth, earth.radius + 400000000);
    testcraft4.trajectory = &t3;
    testcraft4.name = "SS Test 4";
    sol.spacecraft.append(&testcraft4);

    // set up timing
    universe_updater = new std::thread(universe_timing_loop);
    universe_updater->detach();
}

// delta t in time units
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
    universe_next_event = INT64_MAX;

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
