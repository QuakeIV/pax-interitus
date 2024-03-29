#ifndef UNITS_H
#define UNITS_H

#include <stdint.h>

// this file is dedicated to converting units to their fixed point counterparts and should serve to normalize across all reference frames
// in particular this will convert into a base 2 multiple of the value in question, for effecient conversion math

#define TIME_SHIFT                 (20)
#define TIME_FACTOR                (1<<TIME_SHIFT)

// conversions

// TODO: probably change naming scheme to something like FIXEDMASS_TO_KG and KG_TO_FIXEDMASS
// to match the superior naming scheme developing on the python API side

//TODO: poor effeciency given current use case in universe
#define MILLISECONDS_TO_TIME(x) ((int64_t)(((double)x)*(TIME_FACTOR/1000.0)))
#define MICROSECONDS_TO_TIME(x) ((int64_t)(((double)x)*(TIME_FACTOR/1000000.0)))
#define SECONDS_TO_FIXEDTIME(x) ((int64_t)(((double)x)*(TIME_FACTOR)))
#define FIXEDTIME_TO_SECONDS(x) (((double)x)/TIME_FACTOR)

#define RELATIVE_DIALECTRIC_TO_ABSOLUTE(x) (0.000000000008854*((double)x))
#define VOLT_UM_TO_VOLT_M(x) (1000000.0*((double)x))

// TODO: actual fancy int->double conversion bitfield?
#define DISTANCE_FIXED_TO_M(x) (((double)x)/1024.0)
#define DISTANCE_M_TO_FIXED(x) ((int64_t)((x)*1024.0))

#define CUBIC_METER_TO_LITER(x) ((x)*1000)

#define FIXEDMASS_TO_KG(x) (((double)x)/1024.0)
#define KG_TO_FIXEDMASS(x) ((x)*1024) // TODO: ensure better casting?

const double G = 0.000000000066743; //gravitational constant (standard SI)


#endif // UNITS_H
