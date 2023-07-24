#ifndef MINERALS_H
#define MINERALS_H

#include <stdint.h>

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
    // lets say this is 'economic effort per ton'
    // TODO: to be deprecated by a 'deposit' tracking system that tracks accessibility
    int64_t extraction_difficulty;
    // this will probably pretty much stay
    int64_t refinement_difficulty;
} mineral_info;

// TODO: should eventually be a full accounting of the mass of le planette
typedef struct celestial_mineralogy_s
{
    // fixedmass
    __uint128_t quantity_conventional;
    __uint128_t quantity_fuel;
    __uint128_t quantity_duranium;
} celestial_mineralogy;

extern const mineral_info mineral_conventional;
extern const mineral_info mineral_fuel;
extern const mineral_info mineral_duranium;

#ifdef __cplusplus
extern "C"
{
#endif

void mineralogy_init(void);
celestial_mineralogy *generate_mineralogy(__uint128_t mass);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MINERALS_H
