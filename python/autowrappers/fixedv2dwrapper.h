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

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_fixedv2d(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyFixedV2DObject *pyobjectize_fixedv2d(FixedV2D *obj);

extern PyTypeObject PyFixedV2DType;

#endif // PYWRAPPER_FIXEDV2D_H
