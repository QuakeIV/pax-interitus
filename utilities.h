#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <QStringList>
#include <limits.h>
#include "units.h"

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

// convert distance in meters to a string representation
// TODO: this should be config file sensitive eventually
static inline QString get_distance_str(double distance)
{
    if (distance < 0)
        distance *= -1;
    // TODO: make this selectable?
    // metric mode
    //static const QString si_scale[] = {"m", "", "k","M","G","T","P"};
    // aurora mode
    static const QString si_scale[] = {"p", "n", "µ", "m", "", "k","kk","mk","bk"};

    int i = 4;
    while (i > 0 && distance < 1.0)
    {
        distance *= 1000.0;
        i--;
    }
    while (i < sizeof(si_scale)/sizeof(si_scale[0]) && distance > 10000.0)
    {
        distance /= 1000.0;
        i++;
    }

    return QString("%1 %2m").arg(QString::number(distance, 'f', 1), si_scale[i]);
}

// TODO: kindof sucks effeciency wise but i mean this isnt going to happen that much
static inline QString get_time_str(int64_t time)
{
    double seconds = ((double)time) / TIME_FACTOR;
    int64_t mins = seconds / 60;
    int64_t hours = mins / 60;
    int64_t days = hours / 24;
    int64_t years = days / 365;
    days %= 365;
    hours %= 24;
    mins %= 60;
    seconds = fmod(seconds,60.0);

    QString s = "s";
    if (!mins && !hours && !days && !years)
    {
        if (seconds < 1.0)
        {
            seconds *= 1000.0;
            s = "ms";
        }
        if (seconds < 1.0)
        {
            seconds *= 1000.0;
            s = "µs";
        }
    }

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
        list.append(QString::number(seconds,'f',3) + s);
    if (!list.length())
        list.append("0s");


    return list.join(" ");
}

static inline QString get_date_str(void)
{
    extern int64_t universe_time; //aha now i dont need to include universe.h
    int64_t seconds = universe_time >> TIME_SHIFT;
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

static inline QString get_voltage_str(double voltage)
{
    // TODO: make this selectable/configurable?
    static const QString si_scale[] = {"p", "n", "µ", "m", "", "k","M","G","T","P"};
    int i = 4;
    while (i > 0 && voltage < 1.0)
    {
        voltage *= 1000.0;
        i--;
    }
    while (i < sizeof(si_scale)/sizeof(si_scale[0]) && voltage > 10000.0)
    {
        voltage /= 1000.0;
        i++;
    }

    return QString("%1%2V").arg(QString::number(voltage, 'f', 1), si_scale[i]);
}

// currently in joules
static inline QString get_energy_str(double energy)
{
    // TODO: make this selectable/configurable?
    static const QString si_scale[] = {"p", "n", "µ", "m", "", "k","M","G","T","P"};

    int i = 4;
    while (i > 0 && energy < 1.0)
    {
        energy *= 1000.0;
        i--;
    }

    while (i < sizeof(si_scale)/sizeof(si_scale[0]) && energy > 10000.0)
    {
        energy /= 1000.0;
        i++;
    }

    return QString("%1%2J").arg(QString::number(energy, 'f', 1), si_scale[i]);
}

static inline QString get_amperage_str(double amperage)
{
    // TODO: make this selectable/configurable?
    static const QString si_scale[] = {"p", "n", "µ", "m", "", "k","M","G","T","P"};
    int i = 4;
    while (i > 0 && amperage < 1.0)
    {
        amperage *= 1000.0;
        i--;
    }
    while (i < sizeof(si_scale)/sizeof(si_scale[0]) && amperage > 10000.0)
    {
        amperage /= 1000.0;
        i++;
    }

    return QString("%1%2A").arg(QString::number(amperage, 'f', 1), si_scale[i]);
}

// currently in millifarads
static inline QString get_capacitance_str(double capacitance)
{
    // TODO: make this selectable/configurable?
    static const QString si_scale[] = {"p", "n", "µ", "m", "", "k","M","G","T","P"};

    int i = 4;
    while (i > 0 && capacitance < 1.0)
    {
        capacitance *= 1000.0;
        i--;
    }
    while (i < sizeof(si_scale)/sizeof(si_scale[0]) && capacitance > 10000.0)
    {
        capacitance /= 1000.0;
        i++;
    }

    return QString("%1%2F").arg(QString::number(capacitance, 'f', 1), si_scale[i]);
}

// currently in ohms
static inline QString get_resistance_str(double resistance)
{
    // TODO: make this selectable/configurable?
    static const QString si_scale[] = {"p", "n", "µ", "m", "", "k","M","G","T","P"};

    int i = 4;
    while (i > 0 && resistance < 1.0)
    {
        resistance *= 1000.0;
        i--;
    }
    while (i < sizeof(si_scale)/sizeof(si_scale[0]) && resistance > 10000.0)
    {
        resistance /= 1000.0;
        i++;
    }

    return QString("%1%2Ω").arg(QString::number(resistance, 'f', 1), si_scale[i]);
}

// convert area in square meters to a string representation
// TODO: this should be config file sensitive eventually
static inline QString get_area_str(double area)
{
    if (area < 0)
        area *= -1;

    // aurora mode
    // TODO: configurable?
    static const QString si_scale[] = {"p", "n", "µ", "m", "", "k", "kk", "mk", "bk"};

    int i = 4;
    while (i > 0 && area < 0.1)
    {
        area *= 1000000.0;
        i--;
    }
    while (i < sizeof(si_scale)/sizeof(si_scale[0]) && area > 100000.0)
    {
        area /= 1000.0;
        i++;
    }

    // TODO: we might want to actually unify this behavior a bit
    int digits = round(6.0 - log10(area));
    if (digits < 1)
        digits = 1;
    if (digits > 6)
        digits = 6;

    return QString("%1%2m²").arg(QString::number(area, 'f', digits), si_scale[i]);
}

// takes volume in liters
static inline QString get_volume_str(double volume)
{
    // TODO: make this selectable/configurable?
    static const QString si_scale[] = {"p", "n", "µ", "m", "", "k","M","G","T","P"};

    int i = 4;
    while (i > 0 && volume < 1.0)
    {
        volume *= 1000.0;
        i--;
    }
    while (i < sizeof(si_scale)/sizeof(si_scale[0]) && volume > 10000.0)
    {
        volume /= 1000.0;
        i++;
    }

    return QString("%1%2L").arg(QString::number(volume, 'f', 1), si_scale[i]);
}


#endif // UTILITIES_H
