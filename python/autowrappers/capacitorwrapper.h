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

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_capacitor(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyCapacitorObject *pyobjectize_capacitor(Capacitor *obj);

extern PyTypeObject PyCapacitorType;

#endif // PYWRAPPER_CAPACITOR_H
