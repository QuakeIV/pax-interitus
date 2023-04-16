#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "insulatorwrapper.h"
#include "materials.h"

static void type_dealloc(PyInsulatorObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyInsulatorObject *object = (PyInsulatorObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
    {
        object->ref = new Insulator();
    }
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_name(PyInsulatorObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static int set_name(PyInsulatorObject *self, PyObject *value, void *closure)
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
static PyObject* get_density(PyInsulatorObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->density);
}
static int set_density(PyInsulatorObject *self, PyObject *value, void *closure)
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
static PyObject* get_specific_heat(PyInsulatorObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->specific_heat);
}
static int set_specific_heat(PyInsulatorObject *self, PyObject *value, void *closure)
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
static PyObject* get_permittivity(PyInsulatorObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->permittivity);
}
static int set_permittivity(PyInsulatorObject *self, PyObject *value, void *closure)
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
    self->ref->permittivity = v;
    return 0;
}
static PyObject* get_strength(PyInsulatorObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->strength);
}
static int set_strength(PyInsulatorObject *self, PyObject *value, void *closure)
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
    self->ref->strength = v;
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
    "permittivity",
    (getter)get_permittivity,
    (setter)set_permittivity,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "strength",
    (getter)get_strength,
    (setter)set_strength,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls

static PyObject* __eq__(PyInsulatorObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyInsulatorType))
        Py_RETURN_FALSE;
    PyInsulatorObject *other_cast = (PyInsulatorObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyInsulatorType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.Insulator",
    .tp_basicsize = sizeof(PyInsulatorObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython Insulator Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyInsulatorObject *pyobjectize_insulator(Insulator *obj)
{
    wrapper_newup = false;
    PyInsulatorObject *pyobj = (PyInsulatorObject *)PyObject_Call((PyObject *)&PyInsulatorType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_insulator(PyObject *m)
{
    if (PyType_Ready(&PyInsulatorType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyInsulatorType);
    if (PyModule_AddObject(m, "Insulator", (PyObject *)&PyInsulatorType) < 0)
    {
        Py_DECREF(&PyInsulatorType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
