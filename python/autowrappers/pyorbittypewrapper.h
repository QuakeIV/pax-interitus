#ifndef PYWRAPPER_ORBITTYPE_H
#define PYWRAPPER_ORBITTYPE_H

#include <Python.h>

class OrbitType;
typedef struct
{
    PyObject_HEAD
    OrbitType *ref;
    bool tracked;
} PyOrbitTypeObject;

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyOrbitTypeObject *pyobjectize_orbittype(OrbitType *obj);

extern PyTypeObject PyOrbitTypeType;

#endif // PYWRAPPER_ORBITTYPE_H
