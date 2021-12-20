#ifndef FIXEDV2D_H
#define FIXEDV2D_H

class FixedV2D
{
public:
    inline FixedV2D()
    {
        x = 0;
        y = 0;
    }

    inline FixedV2D(long xx, long yy)
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

    long x = 0;
    long y = 0;
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
