#ifndef MINE_H
#define MINE_H

#include "machinery.h"
#include <stdint.h>
#include <QList>

// NOTE: as a balancing factor, could make running without maintenance take a bite out of the lifespan of the device, IE depreciate it massively and incur a much greater long term cost
//   - maybe sometimes thats normal for that type of machine, even?

// TODO: add storage on site for machinery that is being taken offline, store as tuple with condition of the machinery (just clone state at time they were taken offline)
// TODO: different kinds of terrain (ie underwater ice shelf cieling, etc)

// TODO: consider linear rate ramp-up/down, would imply parabolic supply and quantity curves while doing so
// (so actually still pretty thrifty computationally)
// so like if machines run out of supplies, they can ramp down to slow the supply shock
// if machines arrive, they take time to set up

// TODO: sub rate group mines at some point? i dunno it sortof makes sense to do so, though that removes a lot of the need for the functional stuff (meh maybe thats good)

typedef struct mineral_deposit_s mineral_deposit;

// represents a 'mining operation' situated at a mineral deposit
class Mine
{
private:
    // current ore quantity at the mine
    // stored_ore = ore_0 + (universe_time - time_ore_0)*mining_rate
    // 0 time for current ore stockpile curve, for purposes of calculating quantity at any given time
    // fixedtime
    int64_t time_ore_0;
    // ore quantity at time_ore_0
    // fixedmass
    uint64_t stored_ore_0;
    // current unmined ore quantity
    uint64_t unmined_ore_0;

    // supplies present at the mine
    // fixedtime
    int64_t time_supply_0;
    // fixedmass stockpile of maintenance supplies
    uint64_t supply_0;
public:
    // specified mining machinery design, mineral deposit, amount of ore to claim
    Mine(MiningMachineryDesign *m, mineral_deposit *d, uint64_t claim_size);
    ~Mine(void);

    // tell the mine to update its condition at the top of the frame
    void update(void);

    uint64_t take_ore(uint64_t requested_amount);
    void add_supplies(uint64_t amount);
    uint64_t take_supplies(uint64_t amount);
    void add_machinery(uint64_t amount);
    uint64_t take_machinery(uint64_t amount);

    MiningMachineryDesign *machinery_design;
    mineral_deposit *deposit;

    //TODO: store reference to planet/celestial?

    // TODO: actually do this?
    // this would only be useful later for increasing economic depth by providing another
    //   economic sector that is required to support mining operations
    // unitless (for now?)
    // site infrastructure
    //uint64_t needed_infrastructure;
    //uint64_t infrastructure;

    // TODO: finite supply storage capacity?

    // currently active machinery
    // number of machines of the given spec on site
    uint64_t machinery;

    // TODO: storage capacity? currently de facto infinite
    // fixedmass
    uint64_t stored_ore;
    uint64_t claimed_ore;
    uint64_t unmined_ore;
    uint64_t supply;

    // fixedtime, at what point will the currently stored maintenance supplies be depleted
    int64_t time_supply_depletion;
    // fixedtime, when ore is mined out
    int64_t time_ore_depletion;

    bool active;
    bool supply_depletion;
    bool ore_depletion;
};

extern QList<Mine*> all_mines;

#endif // MINE_H
