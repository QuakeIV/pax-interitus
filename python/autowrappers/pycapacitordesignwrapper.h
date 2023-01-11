#ifndef PYWRAPPER_CAPACITORDESIGN_H
#define PYWRAPPER_CAPACITORDESIGN_H

#include <Python.h>

class CapacitorDesign;
typedef struct
{
    PyObject_HEAD
    CapacitorDesign *ref;
    bool tracked;
} PyCapacitorDesignObject;

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyCapacitorDesignObject *pyobjectize_capacitordesign(CapacitorDesign *obj);

extern PyTypeObject PyCapacitorDesignType;

#endif // PYWRAPPER_CAPACITORDESIGN_H
