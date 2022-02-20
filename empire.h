#ifndef EMPIRE_H
#define EMPIRE_H

#include <QList>
#include "materials.h"

class Empire
{
    // TODO: this is most likely the wrong way to do this, this should be wrapped in some metadata type of what is known about it
    QList<Dialectric*> known_dialiectric_materials;
};

#endif // EMPIRE_H
