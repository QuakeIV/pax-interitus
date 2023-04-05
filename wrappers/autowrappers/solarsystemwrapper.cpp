#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "solarsystemwrapper.h"
#include "spacecraftwrapper.h"
#include "celestialwrapper.h"
#include "solarsystem.h"

static void type_dealloc(PySolarSystemObject *self)
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
        PyErr_SetString(PyExc_TypeError, "paxpython.SolarSystem cannot be instantiated from python.");
        return NULL;
    }
    PySolarSystemObject *object = (PySolarSystemObject *)type->tp_alloc(type, 0);
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_name(PySolarSystemObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static int set_name(PySolarSystemObject *self, PyObject *value, void *closure)
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
static PyObject* get_system_id(PySolarSystemObject *self, void *closure)
{
    return PyLong_FromUnsignedLongLong(self->ref->system_id);
}
static int set_system_id(PySolarSystemObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    uint64_t v = PyLong_AsUnsignedLongLong(value);
    PyObject *exception = PyErr_Occurred();
    if (exception)
        return -1;
    self->ref->system_id = v;
    return 0;
}
static PyObject* get_root(PySolarSystemObject *self, void *closure)
{
    return (PyObject*)pyobjectize_celestial(&self->ref->root);
}
static int set_root(PySolarSystemObject *self, PyObject *value, void *closure)
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
    self->ref->root = *v->ref;
    return 0;
}
static PyObject* get_spacecraft(PySolarSystemObject *self, void *closure)
{
    PyObject *spacecraft_pylist = PyList_New(0);
    foreach (Spacecraft *element, self->ref->spacecraft)
    {
        PySpacecraftObject *py_element = pyobjectize_spacecraft(element);
        PyList_Append(spacecraft_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return spacecraft_pylist;
}
static int set_spacecraft(PySolarSystemObject *self, PyObject *value, void *closure)
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
static PyObject* get_celestials(PySolarSystemObject *self, void *closure)
{
    PyObject *celestials_pylist = PyList_New(0);
    foreach (Celestial *element, self->ref->celestials)
    {
        PyCelestialObject *py_element = pyobjectize_celestial(element);
        PyList_Append(celestials_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return celestials_pylist;
}
static int set_celestials(PySolarSystemObject *self, PyObject *value, void *closure)
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
    "system_id",
    (getter)get_system_id,
    (setter)set_system_id,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "root",
    (getter)get_root,
    (setter)set_root,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "spacecraft",
    (getter)get_spacecraft,
    (setter)set_spacecraft,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "celestials",
    (getter)get_celestials,
    (setter)set_celestials,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls

static PyObject* __eq__(PySolarSystemObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PySolarSystemType))
        Py_RETURN_FALSE;
    PySolarSystemObject *other_cast = (PySolarSystemObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PySolarSystemType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.SolarSystem",
    .tp_basicsize = sizeof(PySolarSystemObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython SolarSystem Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PySolarSystemObject *pyobjectize_solarsystem(SolarSystem *obj)
{
    wrapper_newup = false;
    PySolarSystemObject *pyobj = (PySolarSystemObject *)PyObject_Call((PyObject *)&PySolarSystemType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_solarsystem(PyObject *m)
{
    if (PyType_Ready(&PySolarSystemType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PySolarSystemType);
    if (PyModule_AddObject(m, "SolarSystem", (PyObject *)&PySolarSystemType) < 0)
    {
        Py_DECREF(&PySolarSystemType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
