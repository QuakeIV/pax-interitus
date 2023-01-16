#ifndef PYWRAPPER_CELESTIAL_H
#define PYWRAPPER_CELESTIAL_H

#include <Python.h>

class Celestial;
typedef struct
{
    PyObject_HEAD
    Celestial *ref;
    bool tracked;
} PyCelestialObject;

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyCelestialObject *pyobjectize_celestial(Celestial *obj);

extern PyTypeObject PyCelestialType;

#endif // PYWRAPPER_CELESTIAL_H
