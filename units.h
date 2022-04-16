#ifndef UNITS_H
#define UNITS_H

#include <stdint.h>

// this file is dedicated to converting units to their fixed point counterparts and should serve to normalize across all reference frames
// in particular this will convert into a base 2 multiple of the value in question, for effecient conversion math

#define VOLTAGE_SHIFT              (20)
#define VOLTAGE_FACTOR             (1<<VOLTAGE_SHIFT)
#define AMPERAGE_SHIFT             (20)
#define AMPERAGE_FACTOR            (1<<AMPERAGE_SHIFT)
#define DIALECTRIC_CONSTANT_SHIFT  (20) // permittivity
#define DIALECTRIC_CONSTANT_FACTOR (1<<DIALECTRIC_CONSTANT_SHIFT) // permittivity
#define DIALECTRIC_STRENGTH_SHIFT  (20)
#define DIALECTRIC_STRENGTH_FACTOR (1<<DIALECTRIC_STRENGTH_SHIFT)
#define CAPACITANCE_SHIFT          (20)
#define CAPACITANCE_FACTOR         (1<<CAPACITANCE_SHIFT)
#define RESISTANCE_SHIFT           (20)
#define RESISTANCE_FACTOR          (1<<RESISTANCE_SHIFT)
#define ENERGY_SHIFT               (0) //joules
#define ENERGY_FACTOR              (1<<ENERGY_SHIFT)
#define TIME_SHIFT                 (20)
#define TIME_FACTOR                (1<<TIME_SHIFT)
#define PRECISION_DISTANCE_SHIFT   (20)
#define PRECISION_DISTANCE_FACTOR  (1<<PRECISION_DISTANCE_SHIFT)
#define PRECISION_AREA_SHIFT       (20)
#define PRECISION_AREA_FACTOR      (1<<PRECISION_AREA_SHIFT)

// conversions

#define MILLISECONDS_TO_TIME(x) ((int64_t)(((double)x)*(1048576.0/1000.0)))

#endif // UNITS_H
