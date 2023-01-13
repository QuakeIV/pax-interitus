#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "pyorbittypewrapper.h"
#include "pycelestialwrapper.h"
#include "pyfixedv2dwrapper.h"
#include "orbittype.h"

static void type_dealloc(PyOrbitTypeObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    if (wrapper_newup)
    {
        PyErr_SetString(PyExc_TypeError, "paxpython.OrbitType cannot be instantiated from python.");
        return NULL;
    }
    PyOrbitTypeObject *object = (PyOrbitTypeObject *)type->tp_alloc(type, 0);
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_orbital_radius(PyOrbitTypeObject *self, void *closure)
{
    return PyFloat_FromDouble(DISTANCE_FIXED_TO_M(self->ref->orbital_radius));
}
static int set_orbital_radius(PyOrbitTypeObject *self, PyObject *value, void *closure)
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
static PyObject* get_orbital_period(PyOrbitTypeObject *self, void *closure)
{
    return PyFloat_FromDouble(TIME_FIXED_TO_S(self->ref->orbital_period));
}
static int set_orbital_period(PyOrbitTypeObject *self, PyObject *value, void *closure)
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
static PyObject* get_parent(PyOrbitTypeObject *self, void *closure)
{
    if (!self->ref->parent)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_celestial(self->ref->parent);
}
static int set_parent(PyOrbitTypeObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyCelestialType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only set value to Celestial.");
        return -1;
    }
    PyCelestialObject *v = (PyCelestialObject*)value;
    self->ref->parent = v->ref;
    return 0;
}
static PyObject* get_position(PyOrbitTypeObject *self, void *closure)
{
    return (PyObject*)pyobjectize_fixedv2d(&self->ref->position);
}
static int set_position(PyOrbitTypeObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyFixedV2DType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only set value to FixedV2D.");
        return -1;
    }
    PyFixedV2DObject *v = (PyFixedV2DObject*)value;
    self->ref->position = *v->ref;
    return 0;
}
static PyGetSetDef getsets[] = {
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
    {
    "position",
    (getter)get_position,
    (setter)set_position,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls

static PyObject* __eq__(PyOrbitTypeObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyOrbitTypeType))
        Py_RETURN_FALSE;
    PyOrbitTypeObject *other_cast = (PyOrbitTypeObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyOrbitTypeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.OrbitType",
    .tp_basicsize = sizeof(PyOrbitTypeObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython OrbitType Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyOrbitTypeObject *pyobjectize_orbittype(OrbitType *obj)
{
    wrapper_newup = false;
    PyOrbitTypeObject *pyobj = (PyOrbitTypeObject *)PyObject_Call((PyObject *)&PyOrbitTypeType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}
