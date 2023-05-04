#ifndef PYWRAPPER_ENGINE_H
#define PYWRAPPER_ENGINE_H

#include <Python.h>

class Engine;
typedef struct
{
    PyObject_HEAD
    Engine *ref;
    bool tracked;
} PyEngineObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_engine(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyEngineObject *pyobjectize_engine(Engine *obj);

extern PyTypeObject PyEngineType;

#endif // PYWRAPPER_ENGINE_H
