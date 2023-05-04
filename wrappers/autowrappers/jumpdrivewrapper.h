#ifndef PYWRAPPER_JUMPDRIVE_H
#define PYWRAPPER_JUMPDRIVE_H

#include <Python.h>

class Jumpdrive;
typedef struct
{
    PyObject_HEAD
    Jumpdrive *ref;
    bool tracked;
} PyJumpdriveObject;

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_jumpdrive(PyObject *m);

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyJumpdriveObject *pyobjectize_jumpdrive(Jumpdrive *obj);

extern PyTypeObject PyJumpdriveType;

#endif // PYWRAPPER_JUMPDRIVE_H
