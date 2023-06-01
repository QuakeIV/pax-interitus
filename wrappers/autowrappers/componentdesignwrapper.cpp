#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "componentdesignwrapper.h"
#include "circuitdesignwrapper.h"
#include "insulatorwrapper.h"
#include "conductorwrapper.h"
#include "components/component.h"

static void type_dealloc(PyComponentDesignObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyComponentDesignObject *object = (PyComponentDesignObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
    {
        object->ref = new ComponentDesign();
    }
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_name(PyComponentDesignObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static int set_name(PyComponentDesignObject *self, PyObject *value, void *closure)
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
static PyObject* get_uses_power(PyComponentDesignObject *self, void *closure)
{
    return PyBool_FromLong(self->ref->uses_power);
}
static PyObject* get_produces_power(PyComponentDesignObject *self, void *closure)
{
    return PyBool_FromLong(self->ref->produces_power);
}
static PyObject* get_circuit(PyComponentDesignObject *self, void *closure)
{
    if (!self->ref->circuit)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_circuitdesign(self->ref->circuit);
}
static int set_circuit(PyComponentDesignObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyCircuitDesignType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only set value to CircuitDesign.");
        return -1;
    }
    PyCircuitDesignObject *v = (PyCircuitDesignObject*)value;
    self->ref->circuit = v->ref;
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
    "uses_power",
    (getter)get_uses_power,
    NULL, // readonly
    NULL, // documentation string
    NULL, // closure
    },
    {
    "produces_power",
    (getter)get_produces_power,
    NULL, // readonly
    NULL, // documentation string
    NULL, // closure
    },
    {
    "circuit",
    (getter)get_circuit,
    (setter)set_circuit,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls
static PyObject *func_descriptor_string(PyComponentDesignObject *self, PyObject *args)
{
    return PyUnicode_FromString(self->ref->descriptor_string().toStdString().c_str());
}
static PyMethodDef  methods[] = {
    {"descriptor_string", (PyCFunction)func_descriptor_string, METH_NOARGS, PyDoc_STR("Wraps a call to descriptor_string.")},
    {NULL, NULL}
};

static PyObject* __eq__(PyComponentDesignObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyComponentDesignType))
        Py_RETURN_FALSE;
    PyComponentDesignObject *other_cast = (PyComponentDesignObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyComponentDesignType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.ComponentDesign",
    .tp_basicsize = sizeof(PyComponentDesignObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython ComponentDesign Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_methods = methods,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyComponentDesignObject *pyobjectize_componentdesign(ComponentDesign *obj)
{
    wrapper_newup = false;
    PyComponentDesignObject *pyobj = (PyComponentDesignObject *)PyObject_Call((PyObject *)&PyComponentDesignType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_componentdesign(PyObject *m)
{
    if (PyType_Ready(&PyComponentDesignType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyComponentDesignType);
    if (PyModule_AddObject(m, "ComponentDesign", (PyObject *)&PyComponentDesignType) < 0)
    {
        Py_DECREF(&PyComponentDesignType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
