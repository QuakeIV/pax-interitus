#ifndef PYWRAPPER_ENGINEDESIGN_H
#define PYWRAPPER_ENGINEDESIGN_H

#include <Python.h>

class EngineDesign;
typedef struct
{
    PyObject_HEAD
    EngineDesign *ref;
    bool tracked;
} PyEngineDesignObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_enginedesign(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyEngineDesignObject *pyobjectize_enginedesign(EngineDesign *obj);

extern PyTypeObject PyEngineDesignType;

#endif // PYWRAPPER_ENGINEDESIGN_H
