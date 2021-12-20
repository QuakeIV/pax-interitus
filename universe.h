#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <QList>

class Transform;

extern long universe_time;
extern QList<Transform*> transforms;

void universe_update(long delta_t);

#endif // UNIVERSE_H
