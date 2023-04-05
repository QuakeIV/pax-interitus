#ifndef PYWRAPPERS_H
#define PYWRAPPERS_H

#include <Python.h>

#include "autowrappers/insulatorwrapper.h"
#include "autowrappers/capacitordesignwrapper.h"
#include "autowrappers/capacitorwrapper.h"
#include "autowrappers/celestialwrapper.h"
#include "autowrappers/solarsystemwrapper.h"
#include "autowrappers/orbittypewrapper.h"
#include "autowrappers/fixedv2dwrapper.h"
#include "autowrappers/transformwrapper.h"
#include "autowrappers/spacecraftdesignwrapper.h"
#include "autowrappers/spacecraftwrapper.h"

typedef PyObject *(*pyobjectize_generic_func)(void *ref);
typedef struct
{
    PyObject_HEAD
    void *ref;
    bool tracked;
} PyPaxgenericObject;

template<typename> class QList; // forward declare qlist in hopes of not having to include the whole kaboodle (mightve happened anyhow)
typedef struct
{
    PyObject_HEAD
    QList<void*> *ref;
    PyTypeObject *type;
    pyobjectize_generic_func objectize_func;
} PyQListObject;
// Takes pointer to object, returns python wrapper for object with ref count of 1
PyQListObject *pyobjectize_qlist(QList<void*> *ref, PyTypeObject *type, pyobjectize_generic_func objectize_func);
extern PyTypeObject PyQListType;

// tracking global state only, no need for pointers stored in a specialized type
bool init_universewrapper(PyObject *m);
extern PyTypeObject PyUniverseType;

#endif // PYWRAPPERS_H
