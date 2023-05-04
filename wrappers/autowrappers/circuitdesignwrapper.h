#ifndef PYWRAPPER_CIRCUITDESIGN_H
#define PYWRAPPER_CIRCUITDESIGN_H

#include <Python.h>

class CircuitDesign;
typedef struct
{
    PyObject_HEAD
    CircuitDesign *ref;
    bool tracked;
} PyCircuitDesignObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_circuitdesign(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyCircuitDesignObject *pyobjectize_circuitdesign(CircuitDesign *obj);

extern PyTypeObject PyCircuitDesignType;

#endif // PYWRAPPER_CIRCUITDESIGN_H
