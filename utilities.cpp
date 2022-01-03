
#include "utilities.h"

QString distance_to_str(double distance)
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
