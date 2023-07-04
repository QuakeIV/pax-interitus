#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "capacitordesignwrapper.h"
#include "componentdesignwrapper.h"
#include "insulatorwrapper.h"
#include "circuitdesignwrapper.h"
#include "conductorwrapper.h"
#include "components/capacitor.h"

static void type_dealloc(PyCapacitorDesignObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyCapacitorDesignObject *object = (PyCapacitorDesignObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
    {
        object->ref = new CapacitorDesign();
    }
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_name(PyCapacitorDesignObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static int set_name(PyCapacitorDesignObject *self, PyObject *value, void *closure)
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
static PyObject* get_uses_power(PyCapacitorDesignObject *self, void *closure)
{
    return PyBool_FromLong(self->ref->uses_power);
}
static PyObject* get_produces_power(PyCapacitorDesignObject *self, void *closure)
{
    return PyBool_FromLong(self->ref->produces_power);
}
static PyObject* get_circuit(PyCapacitorDesignObject *self, void *closure)
{
    if (!self->ref->circuit)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_circuitdesign(self->ref->circuit);
}
static int set_circuit(PyCapacitorDesignObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyCircuitDesignType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign CircuitDesign type to CapacitorDesign.circuit.");
        return -1;
    }
    PyCircuitDesignObject *v = (PyCircuitDesignObject*)value;
    self->ref->circuit = v->ref;
    v->tracked = true;
    return 0;
}
static PyObject* get_plate_separation(PyCapacitorDesignObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->plate_separation);
}
static int set_plate_separation(PyCapacitorDesignObject *self, PyObject *value, void *closure)
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
    self->ref->plate_separation = v;
    return 0;
}
static PyObject* get_plate_area(PyCapacitorDesignObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->plate_area);
}
static int set_plate_area(PyCapacitorDesignObject *self, PyObject *value, void *closure)
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
    self->ref->plate_area = v;
    return 0;
}
static PyObject* get_resistance(PyCapacitorDesignObject *self, void *closure)
{
    return PyFloat_FromDouble(self->ref->resistance);
}
static int set_resistance(PyCapacitorDesignObject *self, PyObject *value, void *closure)
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
    self->ref->resistance = v;
    return 0;
}
static PyObject* get_insulator(PyCapacitorDesignObject *self, void *closure)
{
    if (!self->ref->insulator)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_insulator(self->ref->insulator);
}
static int set_insulator(PyCapacitorDesignObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyInsulatorType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign Insulator type to CapacitorDesign.insulator.");
        return -1;
    }
    PyInsulatorObject *v = (PyInsulatorObject*)value;
    self->ref->insulator = v->ref;
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
    {
    "plate_separation",
    (getter)get_plate_separation,
    (setter)set_plate_separation,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "plate_area",
    (getter)get_plate_area,
    (setter)set_plate_area,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "resistance",
    (getter)get_resistance,
    (setter)set_resistance,
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
    {NULL},
};

// wrapped function calls
static PyObject *func_descriptor_string(PyCapacitorDesignObject *self, PyObject *args)
{
    return PyUnicode_FromString(self->ref->descriptor_string().toStdString().c_str());
}
static PyObject *func_max_voltage(PyCapacitorDesignObject *self, PyObject *args)
{
    return PyFloat_FromDouble(self->ref->max_voltage());
}
static PyObject *func_capacitance(PyCapacitorDesignObject *self, PyObject *args)
{
    return PyFloat_FromDouble(self->ref->capacitance());
}
static PyObject *func_energy_at_voltage(PyCapacitorDesignObject *self, PyObject *args)
{
    double voltage;
    if (!PyArg_ParseTuple(args, "d", &voltage))
        return NULL;
    return PyFloat_FromDouble(self->ref->energy_at_voltage(voltage));
}
static PyObject *func_max_current(PyCapacitorDesignObject *self, PyObject *args)
{
    double voltage;
    if (!PyArg_ParseTuple(args, "d", &voltage))
        return NULL;
    return PyFloat_FromDouble(self->ref->max_current(voltage));
}
static PyObject *func_charge_time(PyCapacitorDesignObject *self, PyObject *args)
{
    return PyFloat_FromDouble(((double)self->ref->charge_time())/TIME_FACTOR);
}
static PyObject *func_volume(PyCapacitorDesignObject *self, PyObject *args)
{
    return PyFloat_FromDouble(self->ref->volume());
}
static PyObject *func_mass(PyCapacitorDesignObject *self, PyObject *args)
{
    return PyFloat_FromDouble(self->ref->mass());
}
static PyMethodDef  methods[] = {
    {"descriptor_string", (PyCFunction)func_descriptor_string, METH_NOARGS, PyDoc_STR("Wraps a call to descriptor_string.")},
    {"max_voltage", (PyCFunction)func_max_voltage, METH_NOARGS, PyDoc_STR("Wraps a call to max_voltage.")},
    {"capacitance", (PyCFunction)func_capacitance, METH_NOARGS, PyDoc_STR("Wraps a call to capacitance.")},
    {"energy_at_voltage", (PyCFunction)func_energy_at_voltage, METH_VARARGS, PyDoc_STR("Wraps a call to energy_at_voltage.")},
    {"max_current", (PyCFunction)func_max_current, METH_VARARGS, PyDoc_STR("Wraps a call to max_current.")},
    {"charge_time", (PyCFunction)func_charge_time, METH_NOARGS, PyDoc_STR("Wraps a call to charge_time.")},
    {"volume", (PyCFunction)func_volume, METH_NOARGS, PyDoc_STR("Wraps a call to volume.")},
    {"mass", (PyCFunction)func_mass, METH_NOARGS, PyDoc_STR("Wraps a call to mass.")},
    {NULL, NULL}
};

static PyObject* __eq__(PyCapacitorDesignObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyCapacitorDesignType))
        Py_RETURN_FALSE;
    PyCapacitorDesignObject *other_cast = (PyCapacitorDesignObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyCapacitorDesignType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.CapacitorDesign",
    .tp_basicsize = sizeof(PyCapacitorDesignObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython CapacitorDesign Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_methods = methods,
    .tp_getset = getsets,
    .tp_base = &PyComponentDesignType,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyCapacitorDesignObject *pyobjectize_capacitordesign(CapacitorDesign *obj)
{
    wrapper_newup = false;
    PyCapacitorDesignObject *pyobj = (PyCapacitorDesignObject *)PyObject_Call((PyObject *)&PyCapacitorDesignType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_capacitordesign(PyObject *m)
{
    if (PyType_Ready(&PyCapacitorDesignType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PyCapacitorDesignType);
    if (PyModule_AddObject(m, "CapacitorDesign", (PyObject *)&PyCapacitorDesignType) < 0)
    {
        Py_DECREF(&PyCapacitorDesignType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
