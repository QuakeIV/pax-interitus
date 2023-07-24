#ifndef PYWRAPPER_CELESTIAL_MINERALOGY_H
#define PYWRAPPER_CELESTIAL_MINERALOGY_H

#include <Python.h>

typedef struct celestial_mineralogy_s celestial_mineralogy;
typedef struct
{
    PyObject_HEAD
    celestial_mineralogy *ref;
    bool tracked;
} Pycelestial_mineralogyObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_celestial_mineralogy(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
Pycelestial_mineralogyObject *pyobjectize_celestial_mineralogy(celestial_mineralogy *obj);

extern PyTypeObject Pycelestial_mineralogyType;

#endif // PYWRAPPER_CELESTIAL_MINERALOGY_H
