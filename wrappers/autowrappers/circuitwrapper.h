#ifndef PYWRAPPER_CIRCUIT_H
#define PYWRAPPER_CIRCUIT_H

#include <Python.h>

class Circuit;
typedef struct
{
    PyObject_HEAD
    Circuit *ref;
    bool tracked;
} PyCircuitObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_circuit(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyCircuitObject *pyobjectize_circuit(Circuit *obj);

extern PyTypeObject PyCircuitType;

#endif // PYWRAPPER_CIRCUIT_H
