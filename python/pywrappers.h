#ifndef PYWRAPPERS_H
#define PYWRAPPERS_H

#include <Python.h>
#include "universe/celestial.h"
#include "universe/spacecraft/spacecraft.h"

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

#endif // PYWRAPPERS_H
