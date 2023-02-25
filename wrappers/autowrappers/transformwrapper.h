#ifndef PYWRAPPER_TRANSFORM_H
#define PYWRAPPER_TRANSFORM_H

#include <Python.h>

class Transform;
typedef struct
{
    PyObject_HEAD
    Transform *ref;
    bool tracked;
} PyTransformObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_transform(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyTransformObject *pyobjectize_transform(Transform *obj);

extern PyTypeObject PyTransformType;

#endif // PYWRAPPER_TRANSFORM_H
