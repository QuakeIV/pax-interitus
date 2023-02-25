#ifndef PYWRAPPER_INSULATOR_H
#define PYWRAPPER_INSULATOR_H

#include <Python.h>

class Insulator;
typedef struct
{
    PyObject_HEAD
    Insulator *ref;
    bool tracked;
} PyInsulatorObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_insulator(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyInsulatorObject *pyobjectize_insulator(Insulator *obj);

extern PyTypeObject PyInsulatorType;

#endif // PYWRAPPER_INSULATOR_H
