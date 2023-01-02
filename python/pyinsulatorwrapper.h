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

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyInsulatorObject *pyobjectize_insulator(Insulator *obj);

extern PyTypeObject PyInsulatorType;

#endif // PYWRAPPER_INSULATOR_H
