#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "orbitwrapper.h"
#include "celestialwrapper.h"
#include "fixedv2dwrapper.h"
#include "solarsystemwrapper.h"
#include "celestial_mineralogywrapper.h"
#include "spacecraftwrapper.h"
#include "spacecraftdesignwrapper.h"
#include "enginewrapper.h"
#include "reactorwrapper.h"
#include "directedweaponwrapper.h"
#include "jumpdrivewrapper.h"
#include "circuitwrapper.h"
#include "enginedesignwrapper.h"
#include "reactordesignwrapper.h"
#include "directedweapondesignwrapper.h"
#include "jumpdrivedesignwrapper.h"
#include "circuitdesignwrapper.h"
#include "componentwrapper.h"
#include "componentdesignwrapper.h"
#include "capacitordesignwrapper.h"
#include "insulatorwrapper.h"
#include "conductorwrapper.h"
#include "orbit.h"

static void type_dealloc(PyOrbitObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyOrbitObject *object = (PyOrbitObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
    {
        PyCelestialObject *p_pytype;
        double r;
        PyFixedV2DObject *pos_pytype;
        if (!PyArg_ParseTuple(args, "O!dO!", &PyCelestialType, &p_pytype, &r, &PyFixedV2DType, &pos_pytype))
            return NULL;
        Celestial *p = p_pytype->ref;
        FixedV2D *pos = pos_pytype->ref;
        object->ref = new Orbit(p,r,pos);
    }
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_position(PyOrbitObject *self, void *closure)
{
    if (!self->ref->position)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_fixedv2d(self->ref->position);
}
static int set_position(PyOrbitObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyFixedV2DType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign FixedV2D type to Orbit.position.");
        return -1;
    }
    PyFixedV2DObject *v = (PyFixedV2DObject*)value;
    self->ref->position = v->ref;
    v->tracked = true;
    return 0;
}
static PyObject* get_solarsystem(PyOrbitObject *self, void *closure)
{
    if (!self->ref->solarsystem)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_solarsystem(self->ref->solarsystem);
}
static int set_solarsystem(PyOrbitObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PySolarSystemType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign SolarSystem type to Orbit.solarsystem.");
        return -1;
    }
    PySolarSystemObject *v = (PySolarSystemObject*)value;
    self->ref->solarsystem = v->ref;
    v->tracked = true;
    return 0;
}
static PyObject* get_orbital_radius(PyOrbitObject *self, void *closure)
{
    return PyFloat_FromDouble(DISTANCE_FIXED_TO_M(self->ref->orbital_radius));
}
static int set_orbital_radius(PyOrbitObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    PyErr_SetString(PyExc_NotImplementedError, "Setter for bool type not implemented.");
    return -1;
    return 0;
}
static PyObject* get_orbital_period(PyOrbitObject *self, void *closure)
{
    return PyFloat_FromDouble(TIME_FIXED_TO_S(self->ref->orbital_period));
}
static int set_orbital_period(PyOrbitObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    double v = PyFloat_AsDouble(value);
    PyObject *exception = PyErr_Occurred();
    if (exception)
        return -1;
    self->ref->orbital_period = SECONDS_TO_TIME(v);
    return 0;
}
static PyObject* get_parent(PyOrbitObject *self, void *closure)
{
    if (!self->ref->parent)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_celestial(self->ref->parent);
}
static int set_parent(PyOrbitObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyCelestialType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign Celestial type to Orbit.parent.");
        return -1;
    }
    PyCelestialObject *v = (PyCelestialObject*)value;
    self->ref->parent = v->ref;
    v->tracked = true;
    return 0;
}
static PyGetSetDef getsets[] = {
    {
    "position",
    (getter)get_position,
    (setter)set_position,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "solarsystem",
    (getter)get_solarsystem,
    (setter)set_solarsystem,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "orbital_radius",
    (getter)get_orbital_radius,
    (setter)set_orbital_radius,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "orbital_period",
    (getter)get_orbital_period,
    (setter)set_orbital_period,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "parent",
    (getter)get_parent,
    (setter)set_parent,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls

static PyObject* __eq__(PyOrbitObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyOrbitType))
        Py_RETURN_FALSE;
    PyOrbitObject *other_cast = (PyOrbitObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyOrbitType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.Orbit",
    .tp_basicsize = sizeof(PyOrbitObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython Orbit Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyOrbitObject *pyobjectize_orbit(Orbit *obj)
{
    wrapper_newup = false;
    PyOrbitObject *pyobj = (PyOrbitObject *)PyObject_Call((PyObject *)&PyOrbitType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_orbit(PyObject *m)
{
    if (PyType_Ready(&PyOrbitType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyOrbitType);
    if (PyModule_AddObject(m, "Orbit", (PyObject *)&PyOrbitType) < 0)
    {
        Py_DECREF(&PyOrbitType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
