#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "pyinsulatorwrapper.h"
#include "materials.h"

static void type_dealloc(PyInsulatorObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyInsulatorObject *object = (PyInsulatorObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
        object->ref = new Insulator();
    object->tracked = false;
    return (PyObject*)object;
}
static PyObject* get_name(PyInsulatorObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static PyObject* get_density(PyInsulatorObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->density);
}
static PyObject* get_specific_heat(PyInsulatorObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->specific_heat);
}
static PyObject* get_permittivity(PyInsulatorObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->permittivity);
}
static PyObject* get_strength(PyInsulatorObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->strength);
}
static PyObject* __eq__(PyInsulatorObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyInsulatorType))
        Py_RETURN_FALSE;
    PyInsulatorObject *other_cast = (PyInsulatorObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static PyGetSetDef getsets[] = {
    {
    "name",
    (getter)get_name,
    NULL, // setter
    NULL, // documentation string
    NULL, // closure
    },
    {
    "density",
    (getter)get_density,
    NULL, // setter
    NULL, // documentation string
    NULL, // closure
    },
    {
    "specific_heat",
    (getter)get_specific_heat,
    NULL, // setter
    NULL, // documentation string
    NULL, // closure
    },
    {
    "permittivity",
    (getter)get_permittivity,
    NULL, // setter
    NULL, // documentation string
    NULL, // closure
    },
    {
    "strength",
    (getter)get_strength,
    NULL, // setter
    NULL, // documentation string
    NULL, // closure
    },
};
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
