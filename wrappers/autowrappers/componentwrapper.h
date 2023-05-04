#ifndef PYWRAPPER_COMPONENT_H
#define PYWRAPPER_COMPONENT_H

#include <Python.h>

class Component;
typedef struct
{
    PyObject_HEAD
    Component *ref;
    bool tracked;
} PyComponentObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_component(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyComponentObject *pyobjectize_component(Component *obj);

extern PyTypeObject PyComponentType;

#endif // PYWRAPPER_COMPONENT_H
