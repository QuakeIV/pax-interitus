#include "deposit.h"
#include "units.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: for now just monotonically tick a ticker, later might want to allow for array sparsity
// but for now we want a way to actually procedurally allocate and deallocate this all anyhow
static uint64_t mineralogy_ticker = 0;
// eh 1000 seems good
static celestial_mineralogy mineralogy_array[1000] = { 0 };

void mineralogy_init(void)
{
    // re-init mineralogy array
    mineralogy_ticker = 0;
    memset(mineralogy_array, 0, sizeof(mineralogy_array));
}

// TODO: handle gas giants and stuff differently?
// this whole function is honestly a total hack and should be totally re-designed to not be lame later
// for instance, its probably better to generate a planet with gas giant or rock or
// asteroid or whatever in mind ahead of time, and then go from there (rather than trying to deduce that in this function)
celestial_mineralogy *generate_mineralogy(__uint128_t mass)
{
    if (mineralogy_ticker >= 1000)
    {
        printf("mineralogy array overrun, fix that you retard\n");
        exit(1);
    }
    celestial_mineralogy *alloc = &mineralogy_array[mineralogy_ticker];
    memset(alloc, 0, sizeof(celestial_mineralogy));
    mineralogy_ticker++;

    // i dunno crusty planets are about 1% crust supposedly, so just divide by 128 to produce that approximate result
    mass >>= 7;

    double mass_kg = FIXEDMASS_TO_KG(mass);

//    alloc->quantity_conventional = MASS_KG_TO_FIXED(mass_kg*.75);
//    alloc->quantity_fuel = MASS_KG_TO_FIXED(mass_kg*.2);
//    alloc->quantity_duranium = MASS_KG_TO_FIXED(mass_kg*.05);

    return alloc;
}
