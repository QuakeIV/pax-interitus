#ifndef PYWRAPPER_CAPACITOR_H
#define PYWRAPPER_CAPACITOR_H

#include <Python.h>

class Capacitor;
typedef struct
{
    PyObject_HEAD
    Capacitor *ref;
    bool tracked;
} PyCapacitorObject;

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyCapacitorObject *pyobjectize_capacitor(Capacitor *obj);

extern PyTypeObject PyCapacitorType;

#endif // PYWRAPPER_CAPACITOR_H
