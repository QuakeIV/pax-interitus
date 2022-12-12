#ifndef PYWRAPPER_INSULATOR_H
#define PYWRAPPER_INSULATOR_H

#include <Python.h>

class Insulator;
typedef struct
{
    PyObject_HEAD
    Insulator *ref;
} PyInsulatorObject;

extern PyTypeObject PyInsulatorType;

#endif // PYWRAPPER_INSULATOR_H
