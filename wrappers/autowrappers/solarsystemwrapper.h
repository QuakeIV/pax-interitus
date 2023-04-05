#ifndef PYWRAPPER_SOLARSYSTEM_H
#define PYWRAPPER_SOLARSYSTEM_H

#include <Python.h>

class SolarSystem;
typedef struct
{
    PyObject_HEAD
    SolarSystem *ref;
    bool tracked;
} PySolarSystemObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_solarsystem(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PySolarSystemObject *pyobjectize_solarsystem(SolarSystem *obj);

extern PyTypeObject PySolarSystemType;

#endif // PYWRAPPER_SOLARSYSTEM_H
