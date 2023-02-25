#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "transformwrapper.h"
#include "fixedv2dwrapper.h"
#include "transform.h"

static void type_dealloc(PyTransformObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyTransformObject *object = (PyTransformObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
        object->ref = new Transform();
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_position(PyTransformObject *self, void *closure)
{
    return (PyObject*)pyobjectize_fixedv2d(&self->ref->position);
}
static int set_position(PyTransformObject *self, PyObject *value, void *closure)
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
    "position",
    (getter)get_position,
    (setter)set_position,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls

static PyObject* __eq__(PyTransformObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyTransformType))
        Py_RETURN_FALSE;
    PyTransformObject *other_cast = (PyTransformObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyTransformType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.Transform",
    .tp_basicsize = sizeof(PyTransformObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython Transform Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyTransformObject *pyobjectize_transform(Transform *obj)
{
    wrapper_newup = false;
    PyTransformObject *pyobj = (PyTransformObject *)PyObject_Call((PyObject *)&PyTransformType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_transform(PyObject *m)
{
    if (PyType_Ready(&PyTransformType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyTransformType);
    if (PyModule_AddObject(m, "Transform", (PyObject *)&PyTransformType) < 0)
    {
        Py_DECREF(&PyTransformType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
