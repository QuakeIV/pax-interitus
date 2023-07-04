#ifndef PYWRAPPER_ORBIT_H
#define PYWRAPPER_ORBIT_H

#include <Python.h>

class Orbit;
typedef struct
{
    PyObject_HEAD
    Orbit *ref;
    bool tracked;
} PyOrbitObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_orbit(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyOrbitObject *pyobjectize_orbit(Orbit *obj);

extern PyTypeObject PyOrbitType;

#endif // PYWRAPPER_ORBIT_H
