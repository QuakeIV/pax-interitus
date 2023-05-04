#ifndef PYWRAPPER_REACTORDESIGN_H
#define PYWRAPPER_REACTORDESIGN_H

#include <Python.h>

class ReactorDesign;
typedef struct
{
    PyObject_HEAD
    ReactorDesign *ref;
    bool tracked;
} PyReactorDesignObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_reactordesign(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyReactorDesignObject *pyobjectize_reactordesign(ReactorDesign *obj);

extern PyTypeObject PyReactorDesignType;

#endif // PYWRAPPER_REACTORDESIGN_H
