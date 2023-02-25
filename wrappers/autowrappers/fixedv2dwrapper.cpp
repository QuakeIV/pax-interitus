#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "fixedv2dwrapper.h"
#include "fixedv2d.h"

static void type_dealloc(PyFixedV2DObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyFixedV2DObject *object = (PyFixedV2DObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
        object->ref = new FixedV2D();
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_x(PyFixedV2DObject *self, void *closure)
{
    return PyFloat_FromDouble(DISTANCE_FIXED_TO_M(self->ref->x));
}
static int set_x(PyFixedV2DObject *self, PyObject *value, void *closure)
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
static PyObject* get_y(PyFixedV2DObject *self, void *closure)
{
    return PyFloat_FromDouble(DISTANCE_FIXED_TO_M(self->ref->y));
}
static int set_y(PyFixedV2DObject *self, PyObject *value, void *closure)
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
    "x",
    (getter)get_x,
    (setter)set_x,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "y",
    (getter)get_y,
    (setter)set_y,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls

static PyObject* __eq__(PyFixedV2DObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyFixedV2DType))
        Py_RETURN_FALSE;
    PyFixedV2DObject *other_cast = (PyFixedV2DObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyFixedV2DType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.FixedV2D",
    .tp_basicsize = sizeof(PyFixedV2DObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython FixedV2D Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyFixedV2DObject *pyobjectize_fixedv2d(FixedV2D *obj)
{
    wrapper_newup = false;
    PyFixedV2DObject *pyobj = (PyFixedV2DObject *)PyObject_Call((PyObject *)&PyFixedV2DType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_fixedv2d(PyObject *m)
{
    if (PyType_Ready(&PyFixedV2DType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyFixedV2DType);
    if (PyModule_AddObject(m, "FixedV2D", (PyObject *)&PyFixedV2DType) < 0)
    {
        Py_DECREF(&PyFixedV2DType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
