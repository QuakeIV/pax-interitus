#ifndef CELESTIALTYPE_H
#define CELESTIALTYPE_H

#include <QColor>
#include "orbittype.h"
#include <QList>
#include <QPainter>

class SolarSystemType;

class Celestial
{
public:
    //for stars
    Celestial(int64_t r, uint64_t m);
    Celestial(int64_t r, uint64_t m, int64_t distance, Celestial *parent);

    QString name;

    // TODO: kindof dubious that this is a member, while spacecraft are a pointer
    // this might in all due reality be more effecient, however, as spacecraft
    // may indeed get to have different kinds of transforms with some regularity,
    // whereas planets aught not to (we will see how well that holds up)
    OrbitType trajectory;

    uint64_t mass; //in exagrams (10^18 grams) (quadrillions (10^15) of kgs), ideally later replaced with more parameters later
    int64_t radius; //in mm, radius of the object itself
    
    QColor color;

    QList<Celestial*> children;

    SolarSystemType *system; //what system is the celestial located in
};

#endif // CELESTIALTYPE_H
