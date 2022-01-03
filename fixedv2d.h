#ifndef FIXEDV2D_H
#define FIXEDV2D_H

// TODO: make sure math on the limit is right in light of signedness and so forth
// NOTE: 6521908912666391105 should be considered max x or y coordinate in any direction from 0, as it is
// the max value at which the length of the diagonal of is still expressible as a 64 bit signed integer
// this is about 70% of the otherwise expected range
//
// these limits could actually be expressed as a sphere its just not obvious how that aught to be
// calculated or enforced

#include <QDebug>

class FixedV2D
{
public:
    inline FixedV2D()
    {
        x = 0;
        y = 0;
    }

    inline FixedV2D(int64_t xx, int64_t yy)
    {
        x = xx;
        y = yy;
    }

    inline void operator+=(const FixedV2D &v)
    {
        x += v.x;
        y += v.y;
    }

    inline void operator-=(const FixedV2D &v)
    {
        x -= v.x;
        y -= v.y;
    }

    // TODO: might be possible to set high to max int64 since thats meant to be the max possible diagonal distance, which would halve iterations to converge
    inline int64_t distance(FixedV2D other)
    {
        __int128_t dx = this->x - other.x;
        __int128_t dy = this->y - other.y;
        __uint128_t r2 = dx*dx + dy*dy;
        __uint128_t low = 0;
        __uint128_t high = LLONG_MAX; //__int128_t(-1L); // given assumed coordinate limits, can start with a lower max
        while (high - low > (__uint128_t)1)
        {
            __uint128_t mid = low + ((high - low) >> 1);
            if (mid * mid < r2)
                low = mid;
            else
                high = mid;
        }
        return low;
    }

    inline int64_t length(void)
    {
        __uint128_t r2 = x*x + y*y;
        __uint128_t low = 0;
        __uint128_t high = LLONG_MAX; //__int128_t(-1L); // given assumed coordinate limits, can start with a lower max
        int iterations = 0;
        while (high - low > (__uint128_t)1)
        {
            __uint128_t mid = low + ((high - low) >> 1);
            if (mid * mid < r2)
                low = mid;
            else
                high = mid;
            iterations++;
        }
        qDebug() << iterations;
        return low;
    }

    int64_t x = 0;
    int64_t y = 0;
};

inline FixedV2D operator+(const FixedV2D &v1, const FixedV2D &v2)
{
    return FixedV2D(v1.x + v2.x, v1.y + v2.y);
}

inline FixedV2D operator-(const FixedV2D &v1, const FixedV2D &v2)
{
    return FixedV2D(v1.x - v2.x, v1.y - v2.y);
}

#endif // FIXEDV2D_H
