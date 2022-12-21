#ifndef PYWRAPPERS_H
#define PYWRAPPERS_H

#include <Python.h>

#include "pyinsulatorwrapper.h"

// helper for autogenned wrapers
// TODO: instantiate type as well here? doesnt seem possible need typing data
#define INITIALIZE_WRAPPER(wrapper, r) \
    if (!wrapper->tracked && wrapper->ref) \
        delete wrapper->ref; \
    wrapper->ref = r;

class Celestial;
class Spacecraft;

// planets
typedef struct
{
    PyObject_HEAD
    Celestial *ref;
} PyCelestialObject;
extern PyTypeObject PyCelestialType;

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
