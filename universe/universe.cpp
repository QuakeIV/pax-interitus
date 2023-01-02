#include "universe.h"
#include "transform.h"
#include "celestial.h"
#include "spacecraft/taskgroup.h"
#include "solarsystemtype.h"
#include "spacecraft/spacecraft.h"
#include "units.h"
#include <thread>
#include <chrono>

// this is supposed to prevent the UI/python interface from screwing up datastructures while the universe loop is working on them
// lock_shared and unlock_shared for readonly-lock
std::shared_mutex universe_lock;

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
    static SolarSystemType sol = SolarSystemType(695700000.0, 1988500000000000);
    sol.root.color = QColor(226, 223, 24); // nice sun color
    sol.root.name = "Sol";
    sol.name = "Sol";

    static Celestial mercury = Celestial(2439700.0, 330110000, 57909050000.0, &sol.root);
    mercury.name = "Mercury";

    static Celestial venus = Celestial(6051800.0, 4867500000U, 108208000000.0, &sol.root);
    venus.name = "Venus";
    venus.color = QColor(239, 119, 14);

    static Celestial earth = Celestial(6378100.0, 5972200000U, 149598023000.0, &sol.root);
    earth.color = QColor(19, 33, 219);
    earth.name = "Earth";
    //moon
    static Celestial moon = Celestial(1737400.0, 73420000U, 384399000.0, &earth);
    moon.name = "Luna";

    //mars
    static Celestial mars = Celestial(3389500.0, 641710000U, 227939200000.0, &sol.root);
    mars.name = "Mars";
    mars.color = QColor(242, 76, 26);
    //phobos/deimos
    static Celestial phobos = Celestial(11266.7, 11U, 9376000.0, &mars);
    phobos.name = "Phobos";
    static Celestial deimos = Celestial(6200.0, 1U, 23463200.0, &mars);
    deimos.name = "Deimos";

    static Celestial jupiter = Celestial(69911000.0, 1898200000000, 778570000000.0, &sol.root);
    jupiter.name = "Jupiter";
    jupiter.color = QColor(249, 194, 164);
    static Celestial ganymede = Celestial(2634100.0, 148190000, 1070412000.0, &jupiter); //also sorted by mass
    ganymede.name = "Ganymede";
    static Celestial callisto = Celestial(2410300.0, 107590000, 1882709000.0, &jupiter);
    callisto.name = "Callisto";
    static Celestial io       = Celestial(1821600.0, 89319000,  421700000.0, &jupiter);
    io.name = "Io";
    static Celestial europa   = Celestial(1560800.0, 47999000,  671034000.0, &jupiter);
    europa.name = "Europa";
    static Celestial humalia  = Celestial(139600.0,  4200,      11497400000.0, &jupiter);
    humalia.name = "Humalia";
    static Celestial amalthea = Celestial(167000.0,  2080,      181366000.0, &jupiter);
    amalthea.name = "Amalthea";
    static Celestial elara    = Celestial(79900.0,   870,       11671600000.0, &jupiter);
    elara.name = "Elara";
    static Celestial thebe    = Celestial(98600.0,   430,       222452000.0, &jupiter);
    thebe.name = "Thebe";
    static Celestial pasiphae = Celestial(57800.0,   300,       23119300000.0, &jupiter);
    pasiphae.name = "Pasiphae";
    static Celestial carme    = Celestial(46700.0,   130,       22579900000.0, &jupiter);
    carme.name = "Carme";
    static Celestial sinope   = Celestial(35000.0,   75,        24864100000.0, &jupiter);
    sinope.name = "Sinope";
    static Celestial lysithea = Celestial(42200.0,   63,        11628300000.0, &jupiter);
    lysithea.name = "Lysithea";
    static Celestial metis    = Celestial(43000.0,   36,        128852000.0, &jupiter);
    metis.name = "Metis";
    static Celestial ananke   = Celestial(29100.0,   30,        21042500000.0, &jupiter);
    ananke.name = "Ananke";
    static Celestial leda     = Celestial(21500.0,   6,         11196000000.0, &jupiter);
    leda.name = "Leda";
    static Celestial adrastea = Celestial(16400.0,   2,         129000000.0, &jupiter);
    adrastea.name = "Adrastea";

    static Celestial saturn = Celestial(58232000.0, 568340000000, 1433530000000.0, &sol.root);
    saturn.name = "Saturn";
    saturn.color = QColor(198,160,110);
    static Celestial titan      = Celestial(2574730.0, 134520000, 1221930000.0, &saturn);
    titan.name = "Titan";
    static Celestial rhea       = Celestial(763800.0,  2306518,   527108000.0, &saturn);
    rhea.name = "Rhea";
    static Celestial iapetus    = Celestial(734300.0,  1805635,   3560820000.0, &saturn);
    iapetus.name = "Iapetus";
    static Celestial dione      = Celestial(561400.0,  1095452,   377396000.0, &saturn);
    dione.name = "Dione";
    static Celestial tethys     = Celestial(531100.0,  617449,    294619000.0, &saturn);
    tethys.name = "Tethys";
    static Celestial enceladus  = Celestial(252100.0,  108022,    237950000.0, &saturn);
    enceladus.name = "Enceladus";
    static Celestial mimas      = Celestial(198200.0,  37493,     185404000.0, &saturn);
    static Celestial phoebe     = Celestial(106500.0,  8292,      12905900000.0, &saturn);
    static Celestial hyperion   = Celestial(135000.0,  5619,      1481010000.0, &saturn);
    static Celestial janus      = Celestial(89500.0,   1898,      151472000.0, &saturn);
    static Celestial epimetheus = Celestial(58100.0,   527,       151422000.0, &saturn);
    static Celestial prometheus = Celestial(43100.0,   160,       139380000.0, &saturn);
    static Celestial pandora    = Celestial(40700.0,   137,       141720000.0, &saturn);
    static Celestial siamaq     = Celestial(19650.0,   44,        17937000000.0, &saturn);
    static Celestial helene     = Celestial(17600.0,   25,        377396000.0, &saturn);
    static Celestial albiorix   = Celestial(14300.0,   22,        16222700000.0, &saturn);
    static Celestial telesto    = Celestial(12400.0,   9,         294619000.0, &saturn);
    static Celestial paaliaq    = Celestial(12500.0,   7,         15012800000.0, &saturn);
    static Celestial atlas      = Celestial(15100.0,   7,         137670000.0, &saturn);
    static Celestial calypso    = Celestial(10700.0,   6,         294619000.0, &saturn);
    static Celestial pan        = Celestial(14100.0,   5,         133584000.0, &saturn);
    static Celestial ymir       = Celestial(9500.0,    4,         22841900000.0, &saturn);
    static Celestial kivluq     = Celestial(8500.0,    3,         11307500000.0, &saturn);
    static Celestial tarvos     = Celestial(7500.0,    2,         18243800000.0, &saturn);
    static Celestial ijiraq     = Celestial(8500.0,    1,         11348500000.0, &saturn);

    static Celestial uranus = Celestial(25559000.0, 86810000000, 2870972000000.0, &sol.root);
    uranus.name = "Uranus";
    uranus.color = QColor(141,161,170);
    static Celestial titania   = Celestial(788400.0, 3400000, 435910000.0, &uranus);
    static Celestial oberon    = Celestial(776400.0, 3070000, 583520000.0, &uranus);
    static Celestial umbriel   = Celestial(584700.0, 1275000, 266300000.0, &uranus);
    static Celestial ariel     = Celestial(578900.0, 1251000, 191020000.0, &uranus);
    static Celestial miranda   = Celestial(235800.0, 64000,   129390000.0, &uranus);
    static Celestial puck      = Celestial(81000.0,  2900,    86010000.0, &uranus);
    static Celestial sycorax   = Celestial(78500.0,  2300,    12179400000.0, &uranus);
    sycorax.name = "Sycorax";
    static Celestial portia    = Celestial(67500.0,  1700,    69090000.0, &uranus);
    static Celestial juliet    = Celestial(47000.0,  560,     64350000.0, &uranus);
    static Celestial belinda   = Celestial(45000.0,  490,     75260000.0, &uranus);
    static Celestial cressida  = Celestial(40000.0,  340,     61780000.0, &uranus);
    static Celestial rosalind  = Celestial(36000.0,  250,     69940000.0, &uranus);
    static Celestial caliban   = Celestial(21000.0,  250,     7231100000.0, &uranus);
    static Celestial desdemona = Celestial(32000.0,  180,     62680000.0, &uranus);
    static Celestial bianca    = Celestial(25500.0,  92,      59170000.0, &uranus);
    static Celestial prospero  = Celestial(25000.0,  85,      16276800000.0, &uranus);
    static Celestial setebos   = Celestial(24000.0,  75,      17420400000.0, &uranus);
    setebos.name = "Setebos";
    static Celestial ophelia   = Celestial(21500.0,  53,      53790000.0, &uranus);
    static Celestial cordelia  = Celestial(20000.0,  44,      49770000.0, &uranus);
    static Celestial stephano  = Celestial(16000.0,  22,      8007400000.0, &uranus);
    static Celestial perdita   = Celestial(15000.0,  18,      76400000.0, &uranus);
    static Celestial mab       = Celestial(12500.0,  10,      97700000.0, &uranus);
    static Celestial francisco = Celestial(11000.0,  7,       4282900000.0, &uranus);
    static Celestial margaret  = Celestial(10000.0,  5,       14146700000.0, &uranus);
    static Celestial ferdinand = Celestial(10000.0,  5,       20430000000.0, &uranus);
    static Celestial trinculo  = Celestial(9000.0,   4,       8505200000.0, &uranus);
    static Celestial cupid     = Celestial(9000.0,   4,       74800000.0, &uranus);
    cupid.name = "Cupid";

    static Celestial neptune = Celestial(24622000.0, 102413000000, 4500000000000.0, &sol.root);
    neptune.name = "Neptune";
    neptune.color = QColor(98,122,169);
    // TODO: retrograde orbits
    static Celestial triton    = Celestial(1352600.0, 21390000, 354759000.0, &neptune); //also mass order
    static Celestial proteus   = Celestial(210000.0,  44000,    117646000.0, &neptune);
    static Celestial nereid    = Celestial(178500.0,  27000,    5513800000.0, &neptune);
    static Celestial larissa   = Celestial(97000.0,   4200,     73548000.0, &neptune);
    static Celestial despina   = Celestial(78000.0,   2200,     52526000.0, &neptune);
    static Celestial galatea   = Celestial(87400.0,   2120,     61953000.0, &neptune);
    static Celestial thalassa  = Celestial(40700.0,   350,      50074000.0, &neptune);
    static Celestial naiad     = Celestial(30200.0,   190,      48224000.0, &neptune);
    static Celestial halimede  = Celestial(31000.0,   160,      16681000000.0, &neptune);
    static Celestial neso      = Celestial(30000.0,   150,      50258000000.0, &neptune);
    static Celestial sao       = Celestial(22000.0,   60,       22619000000.0, &neptune);
    sao.name = "Sao";
    static Celestial laomedeia = Celestial(21000.0,   50,       22613000000.0, &neptune);
    static Celestial psamthe   = Celestial(20000.0,   40,       46705000000.0, &neptune);
    psamthe.name = "Psamthe";
    static Celestial hippocamp = Celestial(17400.0,   30,       105283000.0, &neptune);

    //TODO: pluto is actually significantly barycentric with charon
    static Celestial pluto = Celestial(1188300.0, 13030000, 5906380000000.0, &sol.root);
    pluto.name = "Pluto";
    static Celestial charon = Celestial(606000.0, 1587000, 17536000.0, &pluto); // pluto moons by mass (because eh)
    charon.name = "Charon";
    static Celestial nix = Celestial(45000.0, 50, 48694000.0, &pluto);
    nix.name = "Nix";
    static Celestial hydra = Celestial(45000.0, 50, 64738000.0, &pluto);
    hydra.name = "Hydra";
    static Celestial kerberos = Celestial(15000.0, 16, 57783000.0, &pluto);
    kerberos.name = "Kerberos";
    static Celestial styx = Celestial(12000.0, 8, 42656000.0, &pluto);
    styx.name = "Styx";

    static Spacecraft testcraft1 = Spacecraft();
    testcraft1.name = "SS Test 1";
    static Transform epic_static_position = Transform();
    epic_static_position.position.x = -105684699015963;
    epic_static_position.position.y = -27305789478705;
    testcraft1.trajectory = &epic_static_position;
    sol.spacecraft.append(&testcraft1);
    static Spacecraft testcraft2 = Spacecraft();
    static OrbitType t1 = OrbitType(&earth, DISTANCE_FIXED_TO_M(earth.radius) + 400000.0);
    testcraft2.trajectory = &t1;
    testcraft2.name = "SS Test 2";
    sol.spacecraft.append(&testcraft2);
    static Spacecraft testcraft3 = Spacecraft();
    static OrbitType t2 = OrbitType(&earth, DISTANCE_FIXED_TO_M(earth.radius) + 400000.0);
    testcraft3.trajectory = &t2;
    testcraft3.name = "SS Test 3";
    sol.spacecraft.append(&testcraft3);
    static Spacecraft testcraft4 = Spacecraft();
    static OrbitType t3 = OrbitType(&earth, DISTANCE_FIXED_TO_M(earth.radius) + 400000.0);
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

    universe_lock.lock();

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

    universe_lock.unlock();
}
