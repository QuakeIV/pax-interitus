#ifndef FLEETTYPE_H
#define FLEETTYPE_H

#include "orbittype.h"
#include <QString>
#include <QColor>
#include "spacecraft.h"
#include "celestial.h"
#include "solarsystemtype.h"

// TODO: order handling infrastructure

class Taskgroup
{
public:
    static uint64_t group_id; //currently just a counter used to initialize 'name'

    QColor color;

    QString name;

    QList<Spacecraft*> ships;

    Taskgroup(void)
    {
        color = QColor(Qt::yellow);

        name = "Task Force " + QString::number(group_id++);
    }

    ~Taskgroup(void)
    {
    }
};

#endif // FLEETTYPE_H
