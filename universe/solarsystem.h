#ifndef SOLARSYSTEMTYPE_H
#define SOLARSYSTEMTYPE_H

#include <QList>
#include "celestial.h"
#include "spacecraft/spacecraft.h"

class SolarSystem
{
public:
    Celestial root; //root celestial body (this might possibly change later)

    QList<Spacecraft*> spacecraft;
    QList<Celestial*> celestials;

    static uint64_t system_id; //currently just a counter used to initialize 'name'
    QString name;

    // parameters for root celestial object
    SolarSystem(double r, double m);
};

#endif // SOLARSYSTEMTYPE_H
