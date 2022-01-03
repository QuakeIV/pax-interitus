#ifndef CELESTIALTYPE_H
#define CELESTIALTYPE_H

#include <QColor>
#include "orbittype.h"
#include <QList>
#include <QPainter>

class SolarSystemType;

class CelestialType
{
public:
    CelestialType(int64_t r, uint64_t m); //for stars
    CelestialType(int64_t r, uint64_t m, int64_t distance, CelestialType *parent);

    QString name;

    OrbitType trajectory;

    uint64_t mass; //in exagrams (10^18 grams) (quadrillions (10^15) of kgs), ideally later replaced with more parameters later
    int64_t radius; //in mm, radius of the object itself

    QColor color;

    QList<CelestialType*> children;

    SolarSystemType *system; //what system is the celestial located in
};

#endif // CELESTIALTYPE_H
