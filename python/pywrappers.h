#ifndef PYWRAPPERS_H
#define PYWRAPPERS_H

#include <Python.h>

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
