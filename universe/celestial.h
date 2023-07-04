#ifndef CELESTIALTYPE_H
#define CELESTIALTYPE_H

#include <QColor>
#include "orbit.h"
#include <QList>
#include <QPainter>

class SolarSystem;

class Celestial
{
public:
    // TODO: is it really suitable to use radius? this is convenient for now to take KMs but may not always be the case
    // r = radius in meters, m = mass in whatever the mass ref frame was
    // distance = meters
    //for stars
    Celestial(double r, uint64_t m, SolarSystem *system);
    //planets/moons/etc
    Celestial(double r, uint64_t m, double distance, Celestial *parent);

    Celestial *parent;

    QString name;

    FixedV2D position;
    Orbit *trajectory;

    uint64_t mass; //in exagrams (10^18 grams) (quadrillions (10^15) of kgs), ideally later replaced with more parameters later
    int64_t radius; // radius of object itself, in 2^10ths of meters (per distance reference frame in units.h)
    double surface_gravity; // informational, m/s^2
    
    QColor color;

    QList<Celestial*> children;

    SolarSystem *system; //what system is the celestial located in
};

#endif // CELESTIALTYPE_H
