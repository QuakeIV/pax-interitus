#ifndef CELESTIALTYPE_H
#define CELESTIALTYPE_H

#include <QColor>
#include "orbit.h"
#include <QList>
#include <QPainter>

class SolarSystem;
typedef struct celestial_mineralogy_s celestial_mineralogy;

class Celestial
{
public:
    // TODO: is it really suitable to use radius? this is convenient for now to take KMs but may not always be the case
    // r = radius in meters
    // m = mass in kgs
    // distance = meters
    //for stars
    // TODO: need a good way to generate a precise version of this (mass in particular)
    // this generation system will probably need to be totally reworked, such that the
    // celestial is basically dictated its properties from an outside generator algorithm
    Celestial(double r, double m, SolarSystem *s);
    //planets/moons/etc
    Celestial(double r, double m, double distance, Celestial *parent);

    Celestial *parent;

    QString name;

    FixedV2D position;
    Orbit *trajectory;

    // TODO: need a good way to generate a precise version of this
    __uint128_t mass; // in fixedmass, see units.h
    int64_t radius; // radius of object itself, in 2^10ths of meters (per distance reference frame in units.h)
    double surface_gravity; // informational, m/s^2

    celestial_mineralogy *mineralogy;
    
    QColor color;

    QList<Celestial*> children;

    SolarSystem *system; //what system is the celestial located in
};

#endif // CELESTIALTYPE_H
