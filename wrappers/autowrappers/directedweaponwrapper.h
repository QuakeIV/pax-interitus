#ifndef PYWRAPPER_DIRECTEDWEAPON_H
#define PYWRAPPER_DIRECTEDWEAPON_H

#include <Python.h>

class Directedweapon;
typedef struct
{
    PyObject_HEAD
    Directedweapon *ref;
    bool tracked;
} PyDirectedweaponObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_directedweapon(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyDirectedweaponObject *pyobjectize_directedweapon(Directedweapon *obj);

extern PyTypeObject PyDirectedweaponType;

#endif // PYWRAPPER_DIRECTEDWEAPON_H
