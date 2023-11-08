#include "mine.h"
#include "deposit.h"
#include "universe.h"
#include <stdint.h>
#include <limits.h>

#include <QList>

// TODO: implement the actually good iterable container thingy
QList<Mine*> all_mines;


Mine::Mine(MiningMachineryDesign *m, mineral_deposit *d, uint64_t claim_size)
{
    machinery_design = m;
    deposit = d;
    claimed_ore = claim_size;
    unmined_ore = claim_size;
    // TODO: actual checking? may not be worth fussing over tbh. what am i going to do break my own rules?
    deposit->claimed_ore += claim_size;

    machinery = 0;

    time_ore_0 = universe_time;
    unmined_ore_0 = claim_size;
    stored_ore_0 = 0;

    time_ore_depletion = ULONG_MAX;

    time_supply_0 = universe_time;
    time_supply_depletion = 0;
    supply_0 = 0;

    deposit->mines.append(this);
    all_mines.append(this);
}

Mine::~Mine(void)
{
    deposit->mines.removeOne(this);
    all_mines.removeOne(this);
    //TODO: refund remaining unmined ore back to deposit?
    // probably, just feels iffy
}

//uint64_t supply_0;
//uint64_t machinery;
//uint64_t time_ore_0;
//uint64_t stored_ore;
//uint64_t claimed_ore;
//uint64_t unmined_ore;
//int64_t time_supply_depletion;
//int64_t time_ore_depletion;
//int64_t time_unmined_ore_0;
//uint64_t unmined_ore_0;
//int64_t time_supply_0;
//uint64_t supply_0;

// update state when invoked by the universe
// needs to maintain quantities that other modules will use to make decisions
void Mine::update(void)
{
    active = time_ore_depletion > universe_time && time_supply_depletion > universe_time;

    // TODO: maybe dont update this unless someone looks (so make quantity a function call) if its a meaningful performance impact
    if (active)
    {
        uint64_t delta_ore = KG_TO_FIXEDMASS(((double)machinery)*machinery_design->extraction_rate)*FIXEDTIME_TO_SECONDS(universe_time - time_ore_0);
        stored_ore  = stored_ore_0 + delta_ore;
        unmined_ore = unmined_ore_0 - delta_ore;
        supply = supply_0 - KG_TO_FIXEDMASS(((double)machinery)*machinery_design->supply_rate*FIXEDTIME_TO_SECONDS(universe_time - time_supply_0));
        // TODO: doesn't feel right
        supply_depletion = false;
        ore_depletion = false;
    }
    else if (!ore_depletion && time_ore_depletion > universe_time && time_ore_depletion <= time_supply_depletion)
    {
        uint64_t delta_ore = KG_TO_FIXEDMASS(((double)machinery)*machinery_design->extraction_rate)*FIXEDTIME_TO_SECONDS(time_ore_depletion - time_ore_0);
        stored_ore    = stored_ore_0 + delta_ore;
        stored_ore_0  = stored_ore;
        unmined_ore   = unmined_ore_0 - delta_ore;
        unmined_ore_0 = unmined_ore;
        time_ore_0    = time_ore_depletion;
        ore_depletion = true;
    }
    else if (!supply_depletion && time_supply_depletion > universe_time && time_supply_depletion <= time_ore_depletion)
    {
        uint64_t delta_ore = KG_TO_FIXEDMASS(((double)machinery)*machinery_design->extraction_rate)*FIXEDTIME_TO_SECONDS(time_supply_depletion - time_ore_0);
        stored_ore       = stored_ore_0 + delta_ore;
        stored_ore_0     = stored_ore;
        unmined_ore      = unmined_ore_0 - delta_ore;
        unmined_ore_0    = unmined_ore;
        time_ore_0       = time_supply_depletion;
        supply_depletion = true;
    }
}

uint64_t Mine::take_ore(uint64_t requested_amount)
{
    if (requested_amount > stored_ore)
        requested_amount = stored_ore;

    // update stored_ore but only really for sake of the current frame
    // (or if mine is inactive, actually)
    stored_ore -= requested_amount;

    stored_ore_0  = stored_ore;
    unmined_ore_0 = unmined_ore;
    time_ore_0 = universe_time;

    return requested_amount;
}

void Mine::add_supplies(uint64_t amount)
{
    // recalculate supply curve
    supply = supply_0 = supply + amount;
    time_supply_0 = universe_time;

    // reset ore mining clocks if needed
    if (supply_depletion)
    {
        time_ore_0 = universe_time;
        time_ore_depletion = universe_time + SECONDS_TO_FIXEDTIME(FIXEDMASS_TO_KG(stored_ore)/(machinery*machinery_design->extraction_rate));
    }

    // recalculate supply depletion time
    time_supply_depletion = universe_time + SECONDS_TO_FIXEDTIME(FIXEDMASS_TO_KG(supply)/(machinery*machinery_design->supply_rate));
}
uint64_t Mine::take_supplies(uint64_t amount)
{
    if (amount > supply)
        amount = supply;

    // recalculate supply curve
    supply = supply_0 = supply - amount;
    time_supply_0 = universe_time;

    // recalculate supply depletion time
    time_supply_depletion = universe_time + SECONDS_TO_FIXEDTIME(FIXEDMASS_TO_KG(supply)/(machinery*machinery_design->supply_rate));

    return amount;
}

void Mine::add_machinery(uint64_t amount)
{
    // recalculate ore extraction curve
    machinery += amount;
    stored_ore_0 = stored_ore;
    unmined_ore_0 = unmined_ore;
    time_ore_0 = universe_time;

    // recalculate supply curve
    supply_0 = supply;
    time_supply_0 = universe_time;

    // recalculate supply depletion time AND ore depletion time
    time_supply_depletion = universe_time + SECONDS_TO_FIXEDTIME(FIXEDMASS_TO_KG(supply)/(machinery*machinery_design->supply_rate));
    time_ore_depletion    = universe_time + SECONDS_TO_FIXEDTIME(FIXEDMASS_TO_KG(stored_ore)/(machinery*machinery_design->extraction_rate));
}
uint64_t Mine::take_machinery(uint64_t amount)
{
    // TODO: this kind of thing may be rather unnecessary
    if (amount > machinery)
        amount = machinery;

    // recalculate ore extraction curve
    machinery -= amount;
    stored_ore_0 = stored_ore;
    unmined_ore_0 = unmined_ore;
    time_ore_0 = universe_time;

    // recalculate supply curve
    supply_0 = supply;
    time_supply_0 = universe_time;

    // recalculate supply depletion time AND ore depletion time
    time_supply_depletion = universe_time + SECONDS_TO_FIXEDTIME(FIXEDMASS_TO_KG(supply)/(machinery*machinery_design->supply_rate));
    time_ore_depletion    = universe_time + SECONDS_TO_FIXEDTIME(FIXEDMASS_TO_KG(stored_ore)/(machinery*machinery_design->extraction_rate));

    return amount;
}
