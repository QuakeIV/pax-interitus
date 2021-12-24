#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <QList>

class Transform;
class CelestialType;
class SolarSystemType;

extern long universe_time;
extern bool universe_paused;
extern long universe_time_warp;
const long universe_min_warp = -10;
const long universe_max_warp =  24;
extern QList<Transform*> transforms;
extern SolarSystemType sol;

void universe_init(void);
void universe_update(long delta_t);

#endif // UNIVERSE_H
