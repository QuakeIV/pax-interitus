#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <limits.h>

// convert universal distance representation (millimeters) to string representation
// takes a double since the pythagorean distance tends to get calculated in the FPU at the moment
QString distance_to_str(double distance);

// only takes positive numbers (i cant be bothered to represent imaginary numbers)
// TODO: newtons method?
inline uint64_t int_sqrt(__uint128_t i)
{
    __uint128_t low = 0;
    // using ULLONG_MAX because that is the highest possible solution
    __uint128_t high = ULLONG_MAX;
    while (high - low > (__uint128_t)1)
    {
        __uint128_t mid = low + ((high - low) >> 1);
        if (mid * mid < i)
            low = mid;
        else
            high = mid;
    }
    return low;
}

#endif // UTILITIES_H
