#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <QStringList>
#include <limits.h>

// only takes positive numbers (i cant be bothered to represent imaginary numbers)
// TODO: newtons method?
static inline uint64_t int_sqrt(__uint128_t i)
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

// multiply and then divide
// TODO: macro?
static inline int64_t muldiv(int64_t x, int64_t mul, int64_t div)
{
    __int128_t trueval = x;
    trueval*= mul;
    trueval/= div;
    return trueval;
}

// convert universal distance representation (millimeters) to string representation
// takes a double since the pythagorean distance tends to get calculated in the FPU at the moment
// TODO: get a root solver going for integer sqrt (or find an existing one)
//TODO: this shit should be config file sensitive eventually
static inline QString distance_to_str(double distance)
{
    if (distance < 0)
        distance *= -1;
    // TODO: make this selectable?
    // metric mode
    //static const QString si_scale[] = {"m", "", "k","M","G","T"};
    // aurora mode
    static const QString si_scale[] = {"m", "", "k","kk","mk","bk"};
    int i;
    for (i = 0; i < (sizeof(si_scale)/sizeof(si_scale[0]) - 1) && distance > 10000.0; i++)
        distance /= 1000;

    return QString("%1 %2m").arg(QString::number(distance, 'f', 1), si_scale[i]);
}

// TODO: kindof sucks effeciency wise but i mean this isnt going to happen that much
static inline QString time_to_str(int64_t time)
{
    int64_t seconds = time / 1000000;
    int64_t mins = seconds / 60;
    int64_t hours = mins / 60;
    int64_t days = hours / 24;
    int64_t years = days / 365;
    days %= 365;
    hours %= 24;
    mins %= 60;
    seconds %= 60;

    QStringList list;

    if (years)
        list.append(QString::number(years) + " years");
    if (days || list.length())
        list.append(QString::number(days) + " days");
    if (hours || list.length())
        list.append(QString::number(hours) + "h");
    if (mins || list.length())
        list.append(QString::number(mins) + "m");
    if (seconds || list.length())
        list.append(QString::number(seconds) + "s");

    return list.join(" ");
}

static inline QString get_date_str(void)
{
    extern int64_t universe_time; //aha now i dont need to include universe.h
    int64_t seconds = universe_time / 1000000;
    int64_t mins = seconds / 60;
    int64_t hours = mins / 60;
    int64_t days = hours / 24;
    int64_t years = days / 365;
    days %= 365;
    hours %= 24;
    mins %= 60;
    seconds %= 60;

    QStringList list = {"year", QString::number(years),
                        "day", QString::number(days),
                       QString::number(hours) + "h",
                       QString::number(mins) + "m",
                       QString::number(seconds) + "s"};

    return list.join(" ");
}


#endif // UTILITIES_H
