#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <QList>
#include "materials.h"
#include <shared_mutex>

class Transform;
class Celestial;
class SolarSystemType;
class Spacecraft;
class SpacecraftDesign;

// this is supposed to prevent the UI/python interface from screwing up datastructures while the universe loop is working on them
extern std::shared_mutex universe_lock;

extern int64_t universe_time; // microseconds now
// universe time for next event (drives update scheduling)
extern int64_t universe_next_event;
extern bool universe_paused;
extern int64_t universe_time_warp;
const int64_t universe_min_warp = -20;
const int64_t universe_max_warp =  26;
extern QList<Transform*> transforms;
extern QList<SolarSystemType*> systems;
extern QList<Spacecraft*> spacecraft;
// TODO: this should most definitively be stored as part of an empire
extern QList<SpacecraftDesign*> spacecraft_designs;

// master list of extant dialectric materials
extern QList<Insulator*> insulator_materials;

//TODO: temporary
#include "empire.h"
extern Empire player_empire;

void universe_init(void);
void universe_update(int64_t delta_t);
// TODO: maybe this should be a macro instead
static inline void set_universe_next_event(int64_t delta_t)
{
    if (delta_t < universe_next_event)
        universe_next_event = delta_t;
}

#endif // UNIVERSE_H
