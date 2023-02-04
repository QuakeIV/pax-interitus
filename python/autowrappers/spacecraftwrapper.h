#ifndef PYWRAPPER_SPACECRAFT_H
#define PYWRAPPER_SPACECRAFT_H

#include <Python.h>

class Spacecraft;
typedef struct
{
    PyObject_HEAD
    Spacecraft *ref;
    bool tracked;
} PySpacecraftObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_spacecraft(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PySpacecraftObject *pyobjectize_spacecraft(Spacecraft *obj);

extern PyTypeObject PySpacecraftType;

#endif // PYWRAPPER_SPACECRAFT_H
