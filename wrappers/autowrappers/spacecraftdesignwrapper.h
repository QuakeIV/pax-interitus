#ifndef PYWRAPPER_SPACECRAFTDESIGN_H
#define PYWRAPPER_SPACECRAFTDESIGN_H

#include <Python.h>

class SpacecraftDesign;
typedef struct
{
    PyObject_HEAD
    SpacecraftDesign *ref;
    bool tracked;
} PySpacecraftDesignObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_spacecraftdesign(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PySpacecraftDesignObject *pyobjectize_spacecraftdesign(SpacecraftDesign *obj);

extern PyTypeObject PySpacecraftDesignType;

#endif // PYWRAPPER_SPACECRAFTDESIGN_H
