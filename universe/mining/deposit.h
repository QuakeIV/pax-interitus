#ifndef MINING_DEPOSIT_H
#define MINING_DEPOSIT_H

#include <stdint.h>
#include <QList>

class Mine;

typedef struct mineral_deposit_s
{
    // fixedmass of ore
    // although the full mass of the planet is stored as uint128_t, this is only allowed to go up to UINT64_MAX
    // as of writing, this corresponds with 18.01x10^15 kg. for comparison phobos is ~10.66x10^15 kg
    uint64_t total_ore;
    // percentage of ore vs unwanted minerals (0-1 scaled) (think accessibility in aurora)
    // effects effeciency of mining equipment (has to waste time extracting crap that isnt the ore)
    // also later the amount and characteristics of ore dressing machinery needed
    // AKA gangue ratio
    float ore_to_waste_ratio;
    // fixedmass
    // whenever mines update they can increment this with whatever newly collected ore they have
    uint64_t mined_ore;
    // fixedmass
    // mines can drop the amount of ore they have claimed here
    uint64_t claimed_ore;

    // TODO: other parameters
    // depth?
    // hardness? probably thats mainly a parameter of the mineral, but it could be a distribution that is randomized to the deposit (maybe parameterized against depth/pressure or the like)
    //     - what about a deposit thats basically a pile of gravel on the surface, vs a rockface, vs a very deep rockface?

    QList<Mine*> mines;
} mineral_deposit;
// TODO: make map structure that will associate mineral_deposit pointer back to mines using it?
// a map-miss will then represent that that deposit apparently has no mines
// i like this
// hmm how to access from python

// TODO: bah
// TODO: should eventually be a full accounting of the mass of le planette
typedef struct celestial_mineralogy_s
{
    mineral_deposit *deposit_array;
    size_t deposit_count;
    // track overall mineral status (mainly what the python API will ideally be looking at)
    // fixedmass
    __uint128_t total_conventional;
    __uint128_t mined_conventional;
    __uint128_t total_fuel;
    __uint128_t mined_fuel;
    __uint128_t total_duranium;
    __uint128_t mined_duranium;
} celestial_mineralogy;

void mineralogy_init(void);
celestial_mineralogy *generate_mineralogy(__uint128_t mass);

#endif // MINING_DEPOSIT_H
