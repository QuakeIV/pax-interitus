#ifndef PYWRAPPER_CONDUCTOR_H
#define PYWRAPPER_CONDUCTOR_H

#include <Python.h>

class Conductor;
typedef struct
{
    PyObject_HEAD
    Conductor *ref;
    bool tracked;
} PyConductorObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_conductor(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyConductorObject *pyobjectize_conductor(Conductor *obj);

extern PyTypeObject PyConductorType;

#endif // PYWRAPPER_CONDUCTOR_H
