#include "universe.h"
#include "orbit.h"
#include "celestial.h"
#include "spacecraft/taskgroup.h"
#include "solarsystem.h"
#include "spacecraft/spacecraft.h"
#include "units.h"
#include "mining/minerals.h"
#include <thread>
#include <chrono>
#include <QDebug>
#include <QList>

// this is supposed to prevent the UI/python interface from screwing up datastructures while the universe loop is working on them
// lock_shared and unlock_shared for readonly-lock
std::shared_mutex universe_lock;

// TODO: eventually allow selection to change on this
// TODO: tie camera position and windows to this?
Empire player_empire;

// master list of extant dialectric materials
// TODO: is this the right file for this?
QList<Insulator*> insulator_materials;

bool universe_paused = false;
// powers of 2 warp factor
int64_t universe_time_warp = 0;
// TODO: negative time is extremely not-functional
int64_t universe_time;

// universe time for next event (drives update scheduling)
// TODO: we really need to be careful with how this gets set
int64_t universe_next_event = INT64_MAX;

// all trajectories
QList<Orbit*> trajectories;

// track all extant solar systems
QList<SolarSystem*> systems;

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
        // TODO: we need some kind of mechanism to cope with slipping frame times to increase delta-t (possibly just TiDi and note the current dilation factor somewhere, but we might actually be able to improve run rate by intentionally reducing the frame rate in certain circumstances)
        std::chrono::time_point start = std::chrono::system_clock::now();
        universe_update(MICROSECONDS_TO_TIME(frame_time_us));
        std::chrono::time_point end = std::chrono::system_clock::now();
        std::this_thread::sleep_for(std::chrono::microseconds(frame_time_us) - (end-start));

        // lazy diagnosis of frame overrun
        // TODO: (wow this chrono library seems horrible and vastly overweight)
        // not that vital to gun for accurate time, minding (stolen from mainwindow.cpp which now no longer handles this)
        //end = std::chrono::system_clock::now();
        //printf("woden %ld\n", std::chrono::duration_cast<std::chrono::microseconds>(end-start).count());
    }
}

void universe_init(void)
{
    mineralogy_init();

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
    static SolarSystem sol = SolarSystem(695700000.0, 1988500000000000e15);
    sol.root->color = QColor(226, 223, 24); // nice sun color
    sol.root->name = "Sol";
    sol.name = "Sol";

    static Celestial mercury = Celestial(2439700.0, 330110000e15, 57909050000.0, sol.root);
    mercury.name = "Mercury";

    static Celestial venus = Celestial(6051800.0, 4867500000e15, 108208000000.0, sol.root);
    venus.name = "Venus";
    venus.color = QColor(239, 119, 14);

    static Celestial earth = Celestial(6378100.0, 5972200000e15, 149598023000.0, sol.root);
    earth.color = QColor(19, 33, 219);
    earth.name = "Earth";
    //moon
    static Celestial moon = Celestial(1737400.0, 73420000e15, 384399000.0, &earth);
    moon.name = "Luna";

    //mars
    static Celestial mars = Celestial(3389500.0, 641710000e15, 227939200000.0, sol.root);
    mars.name = "Mars";
    mars.color = QColor(242, 76, 26);
    //phobos/deimos
    static Celestial phobos = Celestial(11266.7, 11e15, 9376000.0, &mars);
    phobos.name = "Phobos";
    static Celestial deimos = Celestial(6200.0, 1e15, 23463200.0, &mars);
    deimos.name = "Deimos";

    static Celestial jupiter = Celestial(69911000.0, 1898200000000e15, 778570000000.0, sol.root);
    jupiter.name = "Jupiter";
    jupiter.color = QColor(249, 194, 164);
    static Celestial ganymede = Celestial(2634100.0, 148190000e15, 1070412000.0, &jupiter); //also sorted by mass
    ganymede.name = "Ganymede";
    static Celestial callisto = Celestial(2410300.0, 107590000e15, 1882709000.0, &jupiter);
    callisto.name = "Callisto";
    static Celestial io       = Celestial(1821600.0, 89319000e15,  421700000.0, &jupiter);
    io.name = "Io";
    static Celestial europa   = Celestial(1560800.0, 47999000e15,  671034000.0, &jupiter);
    europa.name = "Europa";
    static Celestial humalia  = Celestial(139600.0,  4200e15,      11497400000.0, &jupiter);
    humalia.name = "Humalia";
    static Celestial amalthea = Celestial(167000.0,  2080e15,      181366000.0, &jupiter);
    amalthea.name = "Amalthea";
    static Celestial elara    = Celestial(79900.0,   870e15,       11671600000.0, &jupiter);
    elara.name = "Elara";
    static Celestial thebe    = Celestial(98600.0,   430e15,       222452000.0, &jupiter);
    thebe.name = "Thebe";
    static Celestial pasiphae = Celestial(57800.0,   300e15,       23119300000.0, &jupiter);
    pasiphae.name = "Pasiphae";
    static Celestial carme    = Celestial(46700.0,   130e15,       22579900000.0, &jupiter);
    carme.name = "Carme";
    static Celestial sinope   = Celestial(35000.0,   75e15,        24864100000.0, &jupiter);
    sinope.name = "Sinope";
    static Celestial lysithea = Celestial(42200.0,   63e15,        11628300000.0, &jupiter);
    lysithea.name = "Lysithea";
    static Celestial metis    = Celestial(43000.0,   36e15,        128852000.0, &jupiter);
    metis.name = "Metis";
    static Celestial ananke   = Celestial(29100.0,   30e15,        21042500000.0, &jupiter);
    ananke.name = "Ananke";
    static Celestial leda     = Celestial(21500.0,   6e15,         11196000000.0, &jupiter);
    leda.name = "Leda";
    static Celestial adrastea = Celestial(16400.0,   2e15,         129000000.0, &jupiter);
    adrastea.name = "Adrastea";

    static Celestial saturn = Celestial(58232000.0, 568340000000e15, 1433530000000.0, sol.root);
    saturn.name = "Saturn";
    saturn.color = QColor(198,160,110);
    static Celestial titan      = Celestial(2574730.0, 134520000e15, 1221930000.0, &saturn);
    titan.name = "Titan";
    static Celestial rhea       = Celestial(763800.0,  2306518e15,   527108000.0, &saturn);
    rhea.name = "Rhea";
    static Celestial iapetus    = Celestial(734300.0,  1805635e15,   3560820000.0, &saturn);
    iapetus.name = "Iapetus";
    static Celestial dione      = Celestial(561400.0,  1095452e15,   377396000.0, &saturn);
    dione.name = "Dione";
    static Celestial tethys     = Celestial(531100.0,  617449e15,    294619000.0, &saturn);
    tethys.name = "Tethys";
    static Celestial enceladus  = Celestial(252100.0,  108022e15,    237950000.0, &saturn);
    enceladus.name = "Enceladus";
    static Celestial mimas      = Celestial(198200.0,  37493e15,     185404000.0, &saturn);
    static Celestial phoebe     = Celestial(106500.0,  8292e15,      12905900000.0, &saturn);
    static Celestial hyperion   = Celestial(135000.0,  5619e15,      1481010000.0, &saturn);
    static Celestial janus      = Celestial(89500.0,   1898e15,      151472000.0, &saturn);
    static Celestial epimetheus = Celestial(58100.0,   527e15,       151422000.0, &saturn);
    static Celestial prometheus = Celestial(43100.0,   160e15,       139380000.0, &saturn);
    static Celestial pandora    = Celestial(40700.0,   137e15,       141720000.0, &saturn);
    static Celestial siamaq     = Celestial(19650.0,   44e15,        17937000000.0, &saturn);
    static Celestial helene     = Celestial(17600.0,   25e15,        377396000.0, &saturn);
    static Celestial albiorix   = Celestial(14300.0,   22e15,        16222700000.0, &saturn);
    static Celestial telesto    = Celestial(12400.0,   9e15,         294619000.0, &saturn);
    static Celestial paaliaq    = Celestial(12500.0,   7e15,         15012800000.0, &saturn);
    static Celestial atlas      = Celestial(15100.0,   7e15,         137670000.0, &saturn);
    static Celestial calypso    = Celestial(10700.0,   6e15,         294619000.0, &saturn);
    static Celestial pan        = Celestial(14100.0,   5e15,         133584000.0, &saturn);
    static Celestial ymir       = Celestial(9500.0,    4e15,         22841900000.0, &saturn);
    static Celestial kivluq     = Celestial(8500.0,    3e15,         11307500000.0, &saturn);
    static Celestial tarvos     = Celestial(7500.0,    2e15,         18243800000.0, &saturn);
    static Celestial ijiraq     = Celestial(8500.0,    1e15,         11348500000.0, &saturn);

    static Celestial uranus = Celestial(25559000.0, 86810000000e15, 2870972000000.0, sol.root);
    uranus.name = "Uranus";
    uranus.color = QColor(141,161,170);
    static Celestial titania   = Celestial(788400.0, 3400000e15, 435910000.0, &uranus);
    static Celestial oberon    = Celestial(776400.0, 3070000e15, 583520000.0, &uranus);
    static Celestial umbriel   = Celestial(584700.0, 1275000e15, 266300000.0, &uranus);
    static Celestial ariel     = Celestial(578900.0, 1251000e15, 191020000.0, &uranus);
    static Celestial miranda   = Celestial(235800.0, 64000e15,   129390000.0, &uranus);
    static Celestial puck      = Celestial(81000.0,  2900e15,    86010000.0, &uranus);
    static Celestial sycorax   = Celestial(78500.0,  2300e15,    12179400000.0, &uranus);
    sycorax.name = "Sycorax";
    static Celestial portia    = Celestial(67500.0,  1700e15,    69090000.0, &uranus);
    portia.name = "Portia"; // lol dark matter
    static Celestial juliet    = Celestial(47000.0,  560e15,     64350000.0, &uranus);
    static Celestial belinda   = Celestial(45000.0,  490e15,     75260000.0, &uranus);
    static Celestial cressida  = Celestial(40000.0,  340e15,     61780000.0, &uranus);
    static Celestial rosalind  = Celestial(36000.0,  250e15,     69940000.0, &uranus);
    static Celestial caliban   = Celestial(21000.0,  250e15,     7231100000.0, &uranus);
    static Celestial desdemona = Celestial(32000.0,  180e15,     62680000.0, &uranus);
    desdemona.name = "Desdemona";
    static Celestial bianca    = Celestial(25500.0,  92e15,      59170000.0, &uranus);
    bianca.name = "Bianca";
    static Celestial prospero  = Celestial(25000.0,  85e15,      16276800000.0, &uranus);
    prospero.name = "Prospero";
    static Celestial setebos   = Celestial(24000.0,  75e15,      17420400000.0, &uranus);
    setebos.name = "Setebos";
    static Celestial ophelia   = Celestial(21500.0,  53e15,      53790000.0, &uranus);
    ophelia.name = "Ophelia";
    static Celestial cordelia  = Celestial(20000.0,  44e15,      49770000.0, &uranus);
    cordelia.name = "Cordelia";
    static Celestial stephano  = Celestial(16000.0,  22e15,      8007400000.0, &uranus);
    stephano.name = "Stephano";
    static Celestial perdita   = Celestial(15000.0,  18e15,      76400000.0, &uranus);
    perdita.name = "Perdita";
    static Celestial mab       = Celestial(12500.0,  10e15,      97700000.0, &uranus);
    mab.name = "Mab";
    static Celestial francisco = Celestial(11000.0,  7e15,       4282900000.0, &uranus);
    francisco.name = "Francisco";
    static Celestial margaret  = Celestial(10000.0,  5e15,       14146700000.0, &uranus);
    margaret.name = "Margaret";
    static Celestial ferdinand = Celestial(10000.0,  5e15,       20430000000.0, &uranus);
    ferdinand.name = "Ferdinand";
    static Celestial trinculo  = Celestial(9000.0,   4e15,       8505200000.0, &uranus);
    trinculo.name = "Trinculo";
    static Celestial cupid     = Celestial(9000.0,   4e15,       74800000.0, &uranus);
    cupid.name = "Cupid";

    static Celestial neptune = Celestial(24622000.0, 102413000000e15, 4500000000000.0, sol.root);
    neptune.name = "Neptune";
    neptune.color = QColor(98,122,169);
    // TODO: retrograde orbits
    static Celestial triton    = Celestial(1352600.0, 21390000e15, 354759000.0, &neptune); //also mass order
    triton.name = "Triton";
    static Celestial proteus   = Celestial(210000.0,  44000e15,    117646000.0, &neptune);
    proteus.name = "Proteus";
    static Celestial nereid    = Celestial(178500.0,  27000e15,    5513800000.0, &neptune);
    nereid.name = "Nereid";
    static Celestial larissa   = Celestial(97000.0,   4200e15,     73548000.0, &neptune);
    larissa.name = "Larissa";
    static Celestial despina   = Celestial(78000.0,   2200e15,     52526000.0, &neptune);
    despina.name = "Despina";
    static Celestial galatea   = Celestial(87400.0,   2120e15,     61953000.0, &neptune);
    galatea.name = "Galatea";
    static Celestial thalassa  = Celestial(40700.0,   350e15,      50074000.0, &neptune);
    thalassa.name = "Thalassa";
    static Celestial naiad     = Celestial(30200.0,   190e15,      48224000.0, &neptune);
    naiad.name = "Naiad";
    static Celestial halimede  = Celestial(31000.0,   160e15,      16681000000.0, &neptune);
    halimede.name = "Halimede";
    static Celestial neso      = Celestial(30000.0,   150e15,      50258000000.0, &neptune);
    neso.name = "Neso";
    static Celestial sao       = Celestial(22000.0,   60e15,       22619000000.0, &neptune);
    sao.name = "Sao";
    static Celestial laomedeia = Celestial(21000.0,   50e15,       22613000000.0, &neptune);
    laomedeia.name = "Laomedeia";
    static Celestial psamthe   = Celestial(20000.0,   40e15,       46705000000.0, &neptune);
    psamthe.name = "Psamthe";
    static Celestial hippocamp = Celestial(17400.0,   30e15,       105283000.0, &neptune);

    //TODO: pluto is actually significantly barycentric with charon
    static Celestial pluto = Celestial(1188300.0, 13030000e15, 5906380000000.0, sol.root);
    pluto.name = "Pluto";
    static Celestial charon = Celestial(606000.0, 1587000e15, 17536000.0, &pluto); // pluto moons by mass (because eh)
    charon.name = "Charon";
    static Celestial nix = Celestial(45000.0, 50e15, 48694000.0, &pluto);
    nix.name = "Nix";
    static Celestial hydra = Celestial(45000.0, 50e15, 64738000.0, &pluto);
    hydra.name = "Hydra";
    // based beetle 🐞
    static Celestial kerberos = Celestial(15000.0, 16e15, 57783000.0, &pluto);
    kerberos.name = "Kerberos";
    static Celestial styx = Celestial(12000.0, 8e15, 42656000.0, &pluto);
    styx.name = "Styx";

    static Spacecraft testcraft1 = Spacecraft(&earth, DISTANCE_FIXED_TO_M(earth.radius) + 400000.0);
    testcraft1.name = "SS Test 1";
    sol.spacecraft.append(&testcraft1);
    static Spacecraft testcraft2 = Spacecraft(testcraft1.trajectory);
    testcraft2.name = "SS Test 2";
    sol.spacecraft.append(&testcraft2);
    static Spacecraft testcraft3 = Spacecraft(&earth, DISTANCE_FIXED_TO_M(earth.radius) + 400000.0);
    testcraft3.name = "SS Test 3";
    sol.spacecraft.append(&testcraft3);

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
    foreach (Orbit *t, trajectories)
    {
        t->update_position();
    }

    //TODO: ?

    universe_lock.unlock();
}
