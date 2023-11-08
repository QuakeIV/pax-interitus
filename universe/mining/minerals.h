#ifndef MINERALS_H
#define MINERALS_H

#include <stdint.h>
#include <QList>

class Mine;

// TODO: want minerals composed of desirable materials
// variations will exist wherein certain minerals will more easily provide a desired good, but will have byproducts
//typedef struct
//{
//    // TODO: how to store compositional elements
//    // 0.0-1.0 scale
//    double fraction;
//} composition;

//typedef struct
//{
//    // should be set to some kind of const generally
//    const char *name;
//    // kg per liter (just matching materials.h)
//    double density;
//    // i dunno 10 seems good
//    composition fractions[10];
//} mineral;
// TODO: check note clipboards, rework already pending on this

typedef struct mineral_info_s
{
    const char *name;
    // 'economic effort per kg' or something
    // should be analagous to chemical bonding energy or such, in other words relatively fundamental
    double refinement_difficulty;
    // kg/m3
    // TODO: eh maybe kg/L? use that for volume elsewhere. issue is kg/m3 is the more common representation.  it would be just a base 10 ratio in any case
    // TODO: may want to store inverse density so that a multiply can be used to convert mass to volume (instead of divide)
    double density;
} mineral_info;

extern const mineral_info mineral_conventional;
extern const mineral_info mineral_fuel;
extern const mineral_info mineral_duranium;

#endif // MINERALS_H
