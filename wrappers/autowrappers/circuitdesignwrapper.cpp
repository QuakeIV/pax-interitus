#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "circuitdesignwrapper.h"
#include "insulatorwrapper.h"
#include "conductorwrapper.h"
#include "componentdesignwrapper.h"
#include "components/circuit.h"

static void type_dealloc(PyCircuitDesignObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyCircuitDesignObject *object = (PyCircuitDesignObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
    {
        object->ref = new CircuitDesign();
    }
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_name(PyCircuitDesignObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static int set_name(PyCircuitDesignObject *self, PyObject *value, void *closure)
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
static PyObject* get_insulator(PyCircuitDesignObject *self, void *closure)
{
    if (!self->ref->insulator)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_insulator(self->ref->insulator);
}
static int set_insulator(PyCircuitDesignObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyInsulatorType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign Insulator type to CircuitDesign.insulator.");
        return -1;
    }
    PyInsulatorObject *v = (PyInsulatorObject*)value;
    self->ref->insulator = v->ref;
    v->tracked = true;
    return 0;
}
static PyObject* get_conductor(PyCircuitDesignObject *self, void *closure)
{
    if (!self->ref->conductor)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_conductor(self->ref->conductor);
}
static int set_conductor(PyCircuitDesignObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyConductorType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign Conductor type to CircuitDesign.conductor.");
        return -1;
    }
    PyConductorObject *v = (PyConductorObject*)value;
    self->ref->conductor = v->ref;
    v->tracked = true;
    return 0;
}
static PyObject* get_rated_voltage(PyCircuitDesignObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->rated_voltage);
}
static int set_rated_voltage(PyCircuitDesignObject *self, PyObject *value, void *closure)
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
    self->ref->rated_voltage = v;
    return 0;
}
static PyObject* get_rated_amperage(PyCircuitDesignObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->rated_amperage);
}
static int set_rated_amperage(PyCircuitDesignObject *self, PyObject *value, void *closure)
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
    self->ref->rated_amperage = v;
    return 0;
}
static PyObject* get_components(PyCircuitDesignObject *self, void *closure)
{
    PyObject *components_pylist = PyList_New(0);
    foreach (ComponentDesign *element, self->ref->components)
    {
        PyComponentDesignObject *py_element = pyobjectize_componentdesign(element);
        PyList_Append(components_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return components_pylist;
}
static int set_components(PyCircuitDesignObject *self, PyObject *value, void *closure)
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
    "insulator",
    (getter)get_insulator,
    (setter)set_insulator,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "conductor",
    (getter)get_conductor,
    (setter)set_conductor,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "rated_voltage",
    (getter)get_rated_voltage,
    (setter)set_rated_voltage,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "rated_amperage",
    (getter)get_rated_amperage,
    (setter)set_rated_amperage,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "components",
    (getter)get_components,
    (setter)set_components,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls
static PyObject *func_descriptor_string(PyCircuitDesignObject *self, PyObject *args)
{
    return PyUnicode_FromString(self->ref->descriptor_string().toStdString().c_str());
}
static PyMethodDef  methods[] = {
    {"descriptor_string", (PyCFunction)func_descriptor_string, METH_NOARGS, PyDoc_STR("Wraps a call to descriptor_string.")},
    {NULL, NULL}
};

static PyObject* __eq__(PyCircuitDesignObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyCircuitDesignType))
        Py_RETURN_FALSE;
    PyCircuitDesignObject *other_cast = (PyCircuitDesignObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyCircuitDesignType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.CircuitDesign",
    .tp_basicsize = sizeof(PyCircuitDesignObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython CircuitDesign Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_methods = methods,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyCircuitDesignObject *pyobjectize_circuitdesign(CircuitDesign *obj)
{
    wrapper_newup = false;
    PyCircuitDesignObject *pyobj = (PyCircuitDesignObject *)PyObject_Call((PyObject *)&PyCircuitDesignType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_circuitdesign(PyObject *m)
{
    if (PyType_Ready(&PyCircuitDesignType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyCircuitDesignType);
    if (PyModule_AddObject(m, "CircuitDesign", (PyObject *)&PyCircuitDesignType) < 0)
    {
        Py_DECREF(&PyCircuitDesignType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
