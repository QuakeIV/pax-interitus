#include "universe.h"
#include "transform.h"

long universe_time;

QList<Transform*> transforms;

// delta t in milliseconds
void universe_update(long delta_t)
{
    universe_time += delta_t;

    foreach (Transform *t, transforms)
    {
        t->UpdatePosition();
    }

    //TODO: ?
}
