#ifndef PYWRAPPERS_H
#define PYWRAPPERS_H

#include <Python.h>

#include "autowrappers/pyinsulatorwrapper.h"
#include "autowrappers/pycapacitordesignwrapper.h"
#include "autowrappers/pycapacitorwrapper.h"
#include "autowrappers/pycelestialwrapper.h"
#include "autowrappers/pyorbittypewrapper.h"
#include "autowrappers/pyfixedv2dwrapper.h"

class Spacecraft;

// spacecraft
typedef struct
{
    PyObject_HEAD
    Spacecraft *ref;
} PySpacecraftObject;
extern PyTypeObject PySpacecraftType;

// tracking global state only, no need for pointers stored in a specialized type
extern PyTypeObject PyUniverseType;

#endif // PYWRAPPERS_H
