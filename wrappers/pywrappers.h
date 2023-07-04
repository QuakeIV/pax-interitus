#ifndef PYWRAPPERS_H
#define PYWRAPPERS_H

#include <Python.h>

#include "autowrappers/capacitorwrapper.h"
#include "autowrappers/capacitordesignwrapper.h"
#include "autowrappers/celestialwrapper.h"
#include "autowrappers/circuitwrapper.h"
#include "autowrappers/circuitdesignwrapper.h"
#include "autowrappers/componentwrapper.h"
#include "autowrappers/componentdesignwrapper.h"
#include "autowrappers/conductorwrapper.h"
#include "autowrappers/directedweaponwrapper.h"
#include "autowrappers/directedweapondesignwrapper.h"
#include "autowrappers/enginewrapper.h"
#include "autowrappers/enginedesignwrapper.h"
#include "autowrappers/fixedv2dwrapper.h"
#include "autowrappers/insulatorwrapper.h"
#include "autowrappers/jumpdrivewrapper.h"
#include "autowrappers/jumpdrivedesignwrapper.h"
#include "autowrappers/orbitwrapper.h"
#include "autowrappers/reactorwrapper.h"
#include "autowrappers/reactordesignwrapper.h"
#include "autowrappers/solarsystemwrapper.h"
#include "autowrappers/spacecraftwrapper.h"
#include "autowrappers/spacecraftdesignwrapper.h"

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
