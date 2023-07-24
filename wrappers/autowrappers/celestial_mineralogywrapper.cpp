#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "celestial_mineralogywrapper.h"
#include "universe/minerals.h"

static void type_dealloc(Pycelestial_mineralogyObject *self)
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
        PyErr_SetString(PyExc_TypeError, "paxpython.celestial_mineralogy cannot be instantiated from python.");
        return NULL;
    }
    Pycelestial_mineralogyObject *object = (Pycelestial_mineralogyObject *)type->tp_alloc(type, 0);
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_quantity_conventional(Pycelestial_mineralogyObject *self, void *closure)
{
    return PyFloat_FromDouble(MASS_FIXED_TO_KG(self->ref->quantity_conventional));
}
static int set_quantity_conventional(Pycelestial_mineralogyObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    PyErr_SetString(PyExc_NotImplementedError, "Setter for fixedmass type not implemented.");
    return -1;
    return 0;
}
static PyObject* get_quantity_fuel(Pycelestial_mineralogyObject *self, void *closure)
{
    return PyFloat_FromDouble(MASS_FIXED_TO_KG(self->ref->quantity_fuel));
}
static int set_quantity_fuel(Pycelestial_mineralogyObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    PyErr_SetString(PyExc_NotImplementedError, "Setter for fixedmass type not implemented.");
    return -1;
    return 0;
}
static PyObject* get_quantity_duranium(Pycelestial_mineralogyObject *self, void *closure)
{
    return PyFloat_FromDouble(MASS_FIXED_TO_KG(self->ref->quantity_duranium));
}
static int set_quantity_duranium(Pycelestial_mineralogyObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    PyErr_SetString(PyExc_NotImplementedError, "Setter for fixedmass type not implemented.");
    return -1;
    return 0;
}
static PyGetSetDef getsets[] = {
    {
    "quantity_conventional",
    (getter)get_quantity_conventional,
    (setter)set_quantity_conventional,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "quantity_fuel",
    (getter)get_quantity_fuel,
    (setter)set_quantity_fuel,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "quantity_duranium",
    (getter)get_quantity_duranium,
    (setter)set_quantity_duranium,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls

static PyObject* __eq__(Pycelestial_mineralogyObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&Pycelestial_mineralogyType))
        Py_RETURN_FALSE;
    Pycelestial_mineralogyObject *other_cast = (Pycelestial_mineralogyObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject Pycelestial_mineralogyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.celestial_mineralogy",
    .tp_basicsize = sizeof(Pycelestial_mineralogyObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython celestial_mineralogy Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
Pycelestial_mineralogyObject *pyobjectize_celestial_mineralogy(celestial_mineralogy *obj)
{
    wrapper_newup = false;
    Pycelestial_mineralogyObject *pyobj = (Pycelestial_mineralogyObject *)PyObject_Call((PyObject *)&Pycelestial_mineralogyType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_celestial_mineralogy(PyObject *m)
{
    if (PyType_Ready(&Pycelestial_mineralogyType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&Pycelestial_mineralogyType);
    if (PyModule_AddObject(m, "celestial_mineralogy", (PyObject *)&Pycelestial_mineralogyType) < 0)
    {
        Py_DECREF(&Pycelestial_mineralogyType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
