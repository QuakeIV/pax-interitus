#ifndef PYWRAPPERS_H
#define PYWRAPPERS_H

#include <Python.h>
#include "universe/celestialtype.h"
#include "universe/spacecraft/spacecraft.h"

// planets
typedef struct
{
    PyObject_HEAD
    CelestialType *ref;
} PyCelestialObject;
extern PyTypeObject PyCelestialType;

// spacecraft
typedef struct
{
    PyObject_HEAD
    Spacecraft *ref;
} PySpacecraftObject;
extern PyTypeObject PySpacecraftType;

#endif // PYWRAPPERS_H
