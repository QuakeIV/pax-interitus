#ifndef CELESTIALTYPE_H
#define CELESTIALTYPE_H

#include <QColor>

class OrbitType;

class CelestialType
{
public:
    CelestialType();

    OrbitType *parent;

    unsigned long mass; //in exagrams (10^18 grams) (quadrillions (10^15) of kgs), ideally later replaced with more parameters later
    unsigned long radius; //in mm, radius of the object itself

    QColor color;
};

#endif // CELESTIALTYPE_H
