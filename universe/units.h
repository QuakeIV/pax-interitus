#ifndef UNITS_H
#define UNITS_H

#include <stdint.h>

// this file is dedicated to converting units to their fixed point counterparts and should serve to normalize across all reference frames
// in particular this will convert into a base 2 multiple of the value in question, for effecient conversion math

#define TIME_SHIFT                 (20)
#define TIME_FACTOR                (1<<TIME_SHIFT)

// conversions

//TODO: poor effeciency given current use case in universe
#define MILLISECONDS_TO_TIME(x) ((int64_t)(((double)x)*(1048576.0/1000.0)))
#define MICROSECONDS_TO_TIME(x) ((int64_t)(((double)x)*(1048576.0/1000000.0)))

#define RELATIVE_DIALECTRIC_TO_ABSOLUTE(x) (0.000000000008854*((double)x))
#define VOLT_UM_TO_VOLT_M(x) (1000000.0*((double)x))

// TODO: actual fancy int->double conversion bitfield?
#define DISTANCE_FIXED_TO_FLOAT(x) ((x)/1024.0)
#define DISTANCE_FLOAT_TO_FIXED(x) ((x)*1024.0)

#define CUBIC_METER_TO_LITER(x) ((x)*1000)

#define CELESTIALMASS_TO_GRAMS(x) (((double)x) * 1000000000000000000.0)
#define CELESTIALMASS_TO_KG(x) (((double)x) * 1000000000000000.0)

const double G = 0.000000000066743; //gravitational constant (standard SI)


#endif // UNITS_H
