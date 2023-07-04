#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "enginedesignwrapper.h"
#include "componentdesignwrapper.h"
#include "circuitdesignwrapper.h"
#include "insulatorwrapper.h"
#include "conductorwrapper.h"
#include "components/component.h"

static void type_dealloc(PyEngineDesignObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyEngineDesignObject *object = (PyEngineDesignObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
    {
        object->ref = new EngineDesign();
    }
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_name(PyEngineDesignObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static int set_name(PyEngineDesignObject *self, PyObject *value, void *closure)
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
static PyObject* get_uses_power(PyEngineDesignObject *self, void *closure)
{
    return PyBool_FromLong(self->ref->uses_power);
}
static PyObject* get_produces_power(PyEngineDesignObject *self, void *closure)
{
    return PyBool_FromLong(self->ref->produces_power);
}
static PyObject* get_circuit(PyEngineDesignObject *self, void *closure)
{
    if (!self->ref->circuit)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_circuitdesign(self->ref->circuit);
}
static int set_circuit(PyEngineDesignObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyCircuitDesignType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign CircuitDesign type to EngineDesign.circuit.");
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
static PyObject *func_descriptor_string(PyEngineDesignObject *self, PyObject *args)
{
    return PyUnicode_FromString(self->ref->descriptor_string().toStdString().c_str());
}
static PyMethodDef  methods[] = {
    {"descriptor_string", (PyCFunction)func_descriptor_string, METH_NOARGS, PyDoc_STR("Wraps a call to descriptor_string.")},
    {NULL, NULL}
};

static PyObject* __eq__(PyEngineDesignObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyEngineDesignType))
        Py_RETURN_FALSE;
    PyEngineDesignObject *other_cast = (PyEngineDesignObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyEngineDesignType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.EngineDesign",
    .tp_basicsize = sizeof(PyEngineDesignObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython EngineDesign Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_methods = methods,
    .tp_getset = getsets,
    .tp_base = &PyComponentDesignType,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyEngineDesignObject *pyobjectize_enginedesign(EngineDesign *obj)
{
    wrapper_newup = false;
    PyEngineDesignObject *pyobj = (PyEngineDesignObject *)PyObject_Call((PyObject *)&PyEngineDesignType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_enginedesign(PyObject *m)
{
    if (PyType_Ready(&PyEngineDesignType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyEngineDesignType);
    if (PyModule_AddObject(m, "EngineDesign", (PyObject *)&PyEngineDesignType) < 0)
    {
        Py_DECREF(&PyEngineDesignType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
