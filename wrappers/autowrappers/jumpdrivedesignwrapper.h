#ifndef PYWRAPPER_JUMPDRIVEDESIGN_H
#define PYWRAPPER_JUMPDRIVEDESIGN_H

#include <Python.h>

class JumpdriveDesign;
typedef struct
{
    PyObject_HEAD
    JumpdriveDesign *ref;
    bool tracked;
} PyJumpdriveDesignObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_jumpdrivedesign(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyJumpdriveDesignObject *pyobjectize_jumpdrivedesign(JumpdriveDesign *obj);

extern PyTypeObject PyJumpdriveDesignType;

#endif // PYWRAPPER_JUMPDRIVEDESIGN_H
