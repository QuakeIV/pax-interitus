#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "celestialwrapper.h"
#include "orbitwrapper.h"
#include "fixedv2dwrapper.h"
#include "solarsystemwrapper.h"
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
#include "celestial.h"

static void type_dealloc(PyCelestialObject *self)
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
        PyErr_SetString(PyExc_TypeError, "paxpython.Celestial cannot be instantiated from python.");
        return NULL;
    }
    PyCelestialObject *object = (PyCelestialObject *)type->tp_alloc(type, 0);
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_name(PyCelestialObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static int set_name(PyCelestialObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    const char *c_str = PyUnicode_AsUTF8(value);
    if (!c_str)
        return -1;
    QString v = QString(c_str);
    self->ref->name = v;
    return 0;
}
static PyObject* get_parent(PyCelestialObject *self, void *closure)
{
    if (!self->ref->parent)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_celestial(self->ref->parent);
}
static int set_parent(PyCelestialObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyCelestialType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign Celestial type to Celestial.parent.");
        return -1;
    }
    PyCelestialObject *v = (PyCelestialObject*)value;
    self->ref->parent = v->ref;
    v->tracked = true;
    return 0;
}
static PyObject* get_trajectory(PyCelestialObject *self, void *closure)
{
    if (!self->ref->trajectory)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_orbit(self->ref->trajectory);
}
static int set_trajectory(PyCelestialObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyOrbitType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign Orbit type to Celestial.trajectory.");
        return -1;
    }
    PyOrbitObject *v = (PyOrbitObject*)value;
    self->ref->trajectory = v->ref;
    v->tracked = true;
    return 0;
}
static PyObject* get_mass(PyCelestialObject *self, void *closure)
{
    return PyFloat_FromDouble(CELESTIALMASS_TO_KG(self->ref->mass));
}
static int set_mass(PyCelestialObject *self, PyObject *value, void *closure)
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
static PyObject* get_radius(PyCelestialObject *self, void *closure)
{
    return PyFloat_FromDouble(DISTANCE_FIXED_TO_M(self->ref->radius));
}
static int set_radius(PyCelestialObject *self, PyObject *value, void *closure)
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
static PyObject* get_surface_gravity(PyCelestialObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->surface_gravity);
}
static int set_surface_gravity(PyCelestialObject *self, PyObject *value, void *closure)
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
    self->ref->surface_gravity = v;
    return 0;
}
static PyObject* get_children(PyCelestialObject *self, void *closure)
{
    PyObject *children_pylist = PyList_New(0);
    foreach (Celestial *element, self->ref->children)
    {
        PyCelestialObject *py_element = pyobjectize_celestial(element);
        PyList_Append(children_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return children_pylist;
}
static int set_children(PyCelestialObject *self, PyObject *value, void *closure)
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
static PyGetSetDef getsets[] = {
    {
    "name",
    (getter)get_name,
    (setter)set_name,
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
    "trajectory",
    (getter)get_trajectory,
    (setter)set_trajectory,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "mass",
    (getter)get_mass,
    (setter)set_mass,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "radius",
    (getter)get_radius,
    (setter)set_radius,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "surface_gravity",
    (getter)get_surface_gravity,
    (setter)set_surface_gravity,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "children",
    (getter)get_children,
    (setter)set_children,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls

static PyObject* __eq__(PyCelestialObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyCelestialType))
        Py_RETURN_FALSE;
    PyCelestialObject *other_cast = (PyCelestialObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyCelestialType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.Celestial",
    .tp_basicsize = sizeof(PyCelestialObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython Celestial Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyCelestialObject *pyobjectize_celestial(Celestial *obj)
{
    wrapper_newup = false;
    PyCelestialObject *pyobj = (PyCelestialObject *)PyObject_Call((PyObject *)&PyCelestialType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_celestial(PyObject *m)
{
    if (PyType_Ready(&PyCelestialType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyCelestialType);
    if (PyModule_AddObject(m, "Celestial", (PyObject *)&PyCelestialType) < 0)
    {
        Py_DECREF(&PyCelestialType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
