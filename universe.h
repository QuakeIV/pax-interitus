#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <QList>

class Transform;
class CelestialType;
class SolarSystemType;

extern int64_t universe_time;
extern bool universe_paused;
extern int64_t universe_time_warp;
const int64_t universe_min_warp = -10;
const int64_t universe_max_warp =  24;
extern QList<Transform*> transforms;
extern QList<SolarSystemType*> systems;

void universe_init(void);
void universe_update(int64_t delta_t);

#endif // UNIVERSE_H
