#ifndef PYWRAPPER_COMPONENTDESIGN_H
#define PYWRAPPER_COMPONENTDESIGN_H

#include <Python.h>

class ComponentDesign;
typedef struct
{
    PyObject_HEAD
    ComponentDesign *ref;
    bool tracked;
} PyComponentDesignObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_componentdesign(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyComponentDesignObject *pyobjectize_componentdesign(ComponentDesign *obj);

extern PyTypeObject PyComponentDesignType;

#endif // PYWRAPPER_COMPONENTDESIGN_H
