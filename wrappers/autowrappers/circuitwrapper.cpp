#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "circuitwrapper.h"
#include "circuitdesignwrapper.h"
#include "componentwrapper.h"
#include "insulatorwrapper.h"
#include "conductorwrapper.h"
#include "componentdesignwrapper.h"
#include "spacecraftwrapper.h"
#include "spacecraftdesignwrapper.h"
#include "orbitwrapper.h"
#include "enginewrapper.h"
#include "reactorwrapper.h"
#include "directedweaponwrapper.h"
#include "jumpdrivewrapper.h"
#include "enginedesignwrapper.h"
#include "reactordesignwrapper.h"
#include "directedweapondesignwrapper.h"
#include "jumpdrivedesignwrapper.h"
#include "celestialwrapper.h"
#include "fixedv2dwrapper.h"
#include "capacitordesignwrapper.h"
#include "components/circuit.h"

static void type_dealloc(PyCircuitObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyCircuitObject *object = (PyCircuitObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
    {
        object->ref = new Circuit();
    }
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_design(PyCircuitObject *self, void *closure)
{
    return (PyObject*)pyobjectize_circuitdesign(&self->ref->design);
}
static int set_design(PyCircuitObject *self, PyObject *value, void *closure)
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
    self->ref->design = *v->ref;
    v->tracked = true;
    return 0;
}
static PyObject* get_components(PyCircuitObject *self, void *closure)
{
    PyObject *components_pylist = PyList_New(0);
    foreach (Component *element, self->ref->components)
    {
        PyComponentObject *py_element = pyobjectize_component(element);
        PyList_Append(components_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return components_pylist;
}
static int set_components(PyCircuitObject *self, PyObject *value, void *closure)
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
    "design",
    (getter)get_design,
    (setter)set_design,
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
static PyObject *func_descriptor_string(PyCircuitObject *self, PyObject *args)
{
    return PyUnicode_FromString(self->ref->descriptor_string().toStdString().c_str());
}
static PyMethodDef  methods[] = {
    {"descriptor_string", (PyCFunction)func_descriptor_string, METH_NOARGS, PyDoc_STR("Wraps a call to descriptor_string.")},
    {NULL, NULL}
};

static PyObject* __eq__(PyCircuitObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyCircuitType))
        Py_RETURN_FALSE;
    PyCircuitObject *other_cast = (PyCircuitObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyCircuitType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.Circuit",
    .tp_basicsize = sizeof(PyCircuitObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython Circuit Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_methods = methods,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyCircuitObject *pyobjectize_circuit(Circuit *obj)
{
    wrapper_newup = false;
    PyCircuitObject *pyobj = (PyCircuitObject *)PyObject_Call((PyObject *)&PyCircuitType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_circuit(PyObject *m)
{
    if (PyType_Ready(&PyCircuitType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyCircuitType);
    if (PyModule_AddObject(m, "Circuit", (PyObject *)&PyCircuitType) < 0)
    {
        Py_DECREF(&PyCircuitType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
