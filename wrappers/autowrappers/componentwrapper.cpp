#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "componentwrapper.h"
#include "spacecraftwrapper.h"
#include "spacecraftdesignwrapper.h"
#include "orbitwrapper.h"
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
#include "celestialwrapper.h"
#include "fixedv2dwrapper.h"
#include "solarsystemwrapper.h"
#include "componentdesignwrapper.h"
#include "capacitordesignwrapper.h"
#include "insulatorwrapper.h"
#include "conductorwrapper.h"
#include "celestial_mineralogywrapper.h"
#include "components/component.h"

static void type_dealloc(PyComponentObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyComponentObject *object = (PyComponentObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
    {
        object->ref = new Component();
    }
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_name(PyComponentObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static int set_name(PyComponentObject *self, PyObject *value, void *closure)
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
static PyObject* get_parent(PyComponentObject *self, void *closure)
{
    if (!self->ref->parent)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_spacecraft(self->ref->parent);
}
static int set_parent(PyComponentObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PySpacecraftType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign Spacecraft type to Component.parent.");
        return -1;
    }
    PySpacecraftObject *v = (PySpacecraftObject*)value;
    self->ref->parent = v->ref;
    v->tracked = true;
    return 0;
}
static PyObject* get_resistive_load(PyComponentObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->resistive_load);
}
static int set_resistive_load(PyComponentObject *self, PyObject *value, void *closure)
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
    self->ref->resistive_load = v;
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
    "resistive_load",
    (getter)get_resistive_load,
    (setter)set_resistive_load,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls
static PyObject *func_descriptor_string(PyComponentObject *self, PyObject *args)
{
    return PyUnicode_FromString(self->ref->descriptor_string().toStdString().c_str());
}
static PyObject *func_update_voltage(PyComponentObject *self, PyObject *args)
{
    double voltage;
    if (!PyArg_ParseTuple(args, "d", &voltage))
        return NULL;
    self->ref->update_voltage(voltage);
    Py_RETURN_NONE;
}
static PyMethodDef  methods[] = {
    {"descriptor_string", (PyCFunction)func_descriptor_string, METH_NOARGS, PyDoc_STR("Wraps a call to descriptor_string.")},
    {"update_voltage", (PyCFunction)func_update_voltage, METH_VARARGS, PyDoc_STR("Wraps a call to update_voltage.")},
    {NULL, NULL}
};

static PyObject* __eq__(PyComponentObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyComponentType))
        Py_RETURN_FALSE;
    PyComponentObject *other_cast = (PyComponentObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyComponentType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.Component",
    .tp_basicsize = sizeof(PyComponentObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython Component Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_methods = methods,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyComponentObject *pyobjectize_component(Component *obj)
{
    wrapper_newup = false;
    PyComponentObject *pyobj = (PyComponentObject *)PyObject_Call((PyObject *)&PyComponentType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_component(PyObject *m)
{
    if (PyType_Ready(&PyComponentType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyComponentType);
    if (PyModule_AddObject(m, "Component", (PyObject *)&PyComponentType) < 0)
    {
        Py_DECREF(&PyComponentType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
