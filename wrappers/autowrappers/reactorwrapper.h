#ifndef PYWRAPPER_REACTOR_H
#define PYWRAPPER_REACTOR_H

#include <Python.h>

class Reactor;
typedef struct
{
    PyObject_HEAD
    Reactor *ref;
    bool tracked;
} PyReactorObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_reactor(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyReactorObject *pyobjectize_reactor(Reactor *obj);

extern PyTypeObject PyReactorType;

#endif // PYWRAPPER_REACTOR_H
