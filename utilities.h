#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>

// convert universal distance representation (millimeters) to string representation
// takes a double since the pythagorean distance tends to get calculated in the FPU
QString distance_to_str(double distance);

#endif // UTILITIES_H
