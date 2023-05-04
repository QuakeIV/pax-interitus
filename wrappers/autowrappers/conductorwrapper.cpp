#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "conductorwrapper.h"
#include "materials.h"

static void type_dealloc(PyConductorObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyConductorObject *object = (PyConductorObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
    {
        object->ref = new Conductor();
    }
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_name(PyConductorObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static int set_name(PyConductorObject *self, PyObject *value, void *closure)
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
static PyObject* get_density(PyConductorObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->density);
}
static int set_density(PyConductorObject *self, PyObject *value, void *closure)
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
    self->ref->density = v;
    return 0;
}
static PyObject* get_specific_heat(PyConductorObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->specific_heat);
}
static int set_specific_heat(PyConductorObject *self, PyObject *value, void *closure)
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
    self->ref->specific_heat = v;
    return 0;
}
static PyObject* get_resistivity(PyConductorObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->resistivity);
}
static int set_resistivity(PyConductorObject *self, PyObject *value, void *closure)
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
    self->ref->resistivity = v;
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
    "density",
    (getter)get_density,
    (setter)set_density,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "specific_heat",
    (getter)get_specific_heat,
    (setter)set_specific_heat,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "resistivity",
    (getter)get_resistivity,
    (setter)set_resistivity,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls

static PyObject* __eq__(PyConductorObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyConductorType))
        Py_RETURN_FALSE;
    PyConductorObject *other_cast = (PyConductorObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyConductorType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.Conductor",
    .tp_basicsize = sizeof(PyConductorObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython Conductor Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyConductorObject *pyobjectize_conductor(Conductor *obj)
{
    wrapper_newup = false;
    PyConductorObject *pyobj = (PyConductorObject *)PyObject_Call((PyObject *)&PyConductorType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_conductor(PyObject *m)
{
    if (PyType_Ready(&PyConductorType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyConductorType);
    if (PyModule_AddObject(m, "Conductor", (PyObject *)&PyConductorType) < 0)
    {
        Py_DECREF(&PyConductorType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
