#ifndef PYWRAPPER_DIRECTEDWEAPONDESIGN_H
#define PYWRAPPER_DIRECTEDWEAPONDESIGN_H

#include <Python.h>

class DirectedweaponDesign;
typedef struct
{
    PyObject_HEAD
    DirectedweaponDesign *ref;
    bool tracked;
} PyDirectedweaponDesignObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_directedweapondesign(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyDirectedweaponDesignObject *pyobjectize_directedweapondesign(DirectedweaponDesign *obj);

extern PyTypeObject PyDirectedweaponDesignType;

#endif // PYWRAPPER_DIRECTEDWEAPONDESIGN_H
