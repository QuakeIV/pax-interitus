#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "reactorwrapper.h"
#include "componentwrapper.h"
#include "spacecraftwrapper.h"
#include "reactordesignwrapper.h"
#include "spacecraftdesignwrapper.h"
#include "orbitwrapper.h"
#include "enginewrapper.h"
#include "directedweaponwrapper.h"
#include "jumpdrivewrapper.h"
#include "circuitwrapper.h"
#include "componentdesignwrapper.h"
#include "circuitdesignwrapper.h"
#include "enginedesignwrapper.h"
#include "directedweapondesignwrapper.h"
#include "jumpdrivedesignwrapper.h"
#include "celestialwrapper.h"
#include "fixedv2dwrapper.h"
#include "insulatorwrapper.h"
#include "conductorwrapper.h"
#include "capacitordesignwrapper.h"
#include "components/component.h"

static void type_dealloc(PyReactorObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyReactorObject *object = (PyReactorObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
    {
        object->ref = new Reactor();
    }
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_name(PyReactorObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static int set_name(PyReactorObject *self, PyObject *value, void *closure)
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
static PyObject* get_parent(PyReactorObject *self, void *closure)
{
    if (!self->ref->parent)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_spacecraft(self->ref->parent);
}
static int set_parent(PyReactorObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PySpacecraftType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only set value to Spacecraft.");
        return -1;
    }
    PySpacecraftObject *v = (PySpacecraftObject*)value;
    self->ref->parent = v->ref;
    v->tracked = true;
    return 0;
}
static PyObject* get_resistive_load(PyReactorObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->resistive_load);
}
static int set_resistive_load(PyReactorObject *self, PyObject *value, void *closure)
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
static PyObject* get_design(PyReactorObject *self, void *closure)
{
    return (PyObject*)pyobjectize_reactordesign(&self->ref->design);
}
static int set_design(PyReactorObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyReactorDesignType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only set value to ReactorDesign.");
        return -1;
    }
    PyReactorDesignObject *v = (PyReactorDesignObject*)value;
    self->ref->design = *v->ref;
    v->tracked = true;
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
    {
    "design",
    (getter)get_design,
    (setter)set_design,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls
static PyObject *func_descriptor_string(PyReactorObject *self, PyObject *args)
{
    return PyUnicode_FromString(self->ref->descriptor_string().toStdString().c_str());
}
static PyObject *func_update_voltage(PyReactorObject *self, PyObject *args)
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

static PyObject* __eq__(PyReactorObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyReactorType))
        Py_RETURN_FALSE;
    PyReactorObject *other_cast = (PyReactorObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyReactorType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.Reactor",
    .tp_basicsize = sizeof(PyReactorObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython Reactor Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_methods = methods,
    .tp_getset = getsets,
    .tp_base = &PyComponentType,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyReactorObject *pyobjectize_reactor(Reactor *obj)
{
    wrapper_newup = false;
    PyReactorObject *pyobj = (PyReactorObject *)PyObject_Call((PyObject *)&PyReactorType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_reactor(PyObject *m)
{
    if (PyType_Ready(&PyReactorType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyReactorType);
    if (PyModule_AddObject(m, "Reactor", (PyObject *)&PyReactorType) < 0)
    {
        Py_DECREF(&PyReactorType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
