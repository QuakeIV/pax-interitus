#ifndef PYWRAPPERS_H
#define PYWRAPPERS_H

#include <Python.h>
#include "universe/celestialtype.h"

// Python Type
typedef struct
{
    PyObject_HEAD
    CelestialType *ref;
} PyCelestialObject;
extern PyTypeObject PyCelestialType;

#endif // PYWRAPPERS_H
