#ifndef CELESTIALTYPE_H
#define CELESTIALTYPE_H

#include <QColor>
#include "orbittype.h"
#include <QList>
#include <QPainter>

class CelestialType
{
public:
    CelestialType(unsigned long r, unsigned long m); //for stars
    CelestialType(unsigned long r, unsigned long m, unsigned long distance, CelestialType *parent);

    OrbitType trajectory;

    unsigned long mass; //in exagrams (10^18 grams) (quadrillions (10^15) of kgs), ideally later replaced with more parameters later
    unsigned long radius; //in mm, radius of the object itself

    QColor color;

    QList<CelestialType*> children;
};

#endif // CELESTIALTYPE_H