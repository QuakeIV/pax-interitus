#ifndef PYWRAPPER_FIXEDV2D_H
#define PYWRAPPER_FIXEDV2D_H

#include <Python.h>

class FixedV2D;
typedef struct
{
    PyObject_HEAD
    FixedV2D *ref;
    bool tracked;
} PyFixedV2DObject;

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyFixedV2DObject *pyobjectize_fixedv2d(FixedV2D *obj);

extern PyTypeObject PyFixedV2DType;

#endif // PYWRAPPER_FIXEDV2D_H
