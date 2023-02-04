#ifndef PYWRAPPERS_H
#define PYWRAPPERS_H

#include <Python.h>

#include "autowrappers/insulatorwrapper.h"
#include "autowrappers/capacitordesignwrapper.h"
#include "autowrappers/capacitorwrapper.h"
#include "autowrappers/celestialwrapper.h"
#include "autowrappers/orbittypewrapper.h"
#include "autowrappers/fixedv2dwrapper.h"
#include "autowrappers/transformwrapper.h"
#include "autowrappers/spacecraftdesignwrapper.h"
#include "autowrappers/spacecraftwrapper.h"

// tracking global state only, no need for pointers stored in a specialized type
extern PyTypeObject PyUniverseType;

#endif // PYWRAPPERS_H
