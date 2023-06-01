#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "spacecraftdesignwrapper.h"
#include "enginedesignwrapper.h"
#include "reactordesignwrapper.h"
#include "directedweapondesignwrapper.h"
#include "jumpdrivedesignwrapper.h"
#include "circuitdesignwrapper.h"
#include "componentdesignwrapper.h"
#include "capacitordesignwrapper.h"
#include "insulatorwrapper.h"
#include "conductorwrapper.h"
#include "spacecraft/spacecraft.h"

static void type_dealloc(PySpacecraftDesignObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PySpacecraftDesignObject *object = (PySpacecraftDesignObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
    {
        object->ref = new SpacecraftDesign();
    }
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_type(PySpacecraftDesignObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->type.toStdString().c_str());
}
static int set_type(PySpacecraftDesignObject *self, PyObject *value, void *closure)
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
    self->ref->type = v;
    return 0;
}
static PyObject* get_class_name(PySpacecraftDesignObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->class_name.toStdString().c_str());
}
static int set_class_name(PySpacecraftDesignObject *self, PyObject *value, void *closure)
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
    self->ref->class_name = v;
    return 0;
}
static PyObject* get_obsolete(PySpacecraftDesignObject *self, void *closure)
{
    return PyBool_FromLong(self->ref->obsolete);
}
static int set_obsolete(PySpacecraftDesignObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    int v = PyObject_IsTrue(value);
    if (v == -1)
        return -1;
    self->ref->obsolete = v;
    return 0;
}
static PyObject* get_radius(PySpacecraftDesignObject *self, void *closure)
{
    return PyFloat_FromDouble(DISTANCE_FIXED_TO_M(self->ref->radius));
}
static int set_radius(PySpacecraftDesignObject *self, PyObject *value, void *closure)
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
static PyObject* get_engines(PySpacecraftDesignObject *self, void *closure)
{
    PyObject *engines_pylist = PyList_New(0);
    foreach (EngineDesign *element, self->ref->engines)
    {
        PyEngineDesignObject *py_element = pyobjectize_enginedesign(element);
        PyList_Append(engines_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return engines_pylist;
}
static int set_engines(PySpacecraftDesignObject *self, PyObject *value, void *closure)
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
static PyObject* get_reactors(PySpacecraftDesignObject *self, void *closure)
{
    PyObject *reactors_pylist = PyList_New(0);
    foreach (ReactorDesign *element, self->ref->reactors)
    {
        PyReactorDesignObject *py_element = pyobjectize_reactordesign(element);
        PyList_Append(reactors_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return reactors_pylist;
}
static int set_reactors(PySpacecraftDesignObject *self, PyObject *value, void *closure)
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
static PyObject* get_directed_weapons(PySpacecraftDesignObject *self, void *closure)
{
    PyObject *directed_weapons_pylist = PyList_New(0);
    foreach (DirectedweaponDesign *element, self->ref->directed_weapons)
    {
        PyDirectedweaponDesignObject *py_element = pyobjectize_directedweapondesign(element);
        PyList_Append(directed_weapons_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return directed_weapons_pylist;
}
static int set_directed_weapons(PySpacecraftDesignObject *self, PyObject *value, void *closure)
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
static PyObject* get_jump_drives(PySpacecraftDesignObject *self, void *closure)
{
    PyObject *jump_drives_pylist = PyList_New(0);
    foreach (JumpdriveDesign *element, self->ref->jump_drives)
    {
        PyJumpdriveDesignObject *py_element = pyobjectize_jumpdrivedesign(element);
        PyList_Append(jump_drives_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return jump_drives_pylist;
}
static int set_jump_drives(PySpacecraftDesignObject *self, PyObject *value, void *closure)
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
static PyObject* get_circuits(PySpacecraftDesignObject *self, void *closure)
{
    PyObject *circuits_pylist = PyList_New(0);
    foreach (CircuitDesign *element, self->ref->circuits)
    {
        PyCircuitDesignObject *py_element = pyobjectize_circuitdesign(element);
        PyList_Append(circuits_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return circuits_pylist;
}
static int set_circuits(PySpacecraftDesignObject *self, PyObject *value, void *closure)
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
    "type",
    (getter)get_type,
    (setter)set_type,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "class_name",
    (getter)get_class_name,
    (setter)set_class_name,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "obsolete",
    (getter)get_obsolete,
    (setter)set_obsolete,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "radius",
    (getter)get_radius,
    (setter)set_radius,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "engines",
    (getter)get_engines,
    (setter)set_engines,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "reactors",
    (getter)get_reactors,
    (setter)set_reactors,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "directed_weapons",
    (getter)get_directed_weapons,
    (setter)set_directed_weapons,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "jump_drives",
    (getter)get_jump_drives,
    (setter)set_jump_drives,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "circuits",
    (getter)get_circuits,
    (setter)set_circuits,
    NULL, // documentation string
    NULL, // closure
    },
    {NULL},
};

// wrapped function calls

static PyObject* __eq__(PySpacecraftDesignObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PySpacecraftDesignType))
        Py_RETURN_FALSE;
    PySpacecraftDesignObject *other_cast = (PySpacecraftDesignObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PySpacecraftDesignType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.SpacecraftDesign",
    .tp_basicsize = sizeof(PySpacecraftDesignObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython SpacecraftDesign Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PySpacecraftDesignObject *pyobjectize_spacecraftdesign(SpacecraftDesign *obj)
{
    wrapper_newup = false;
    PySpacecraftDesignObject *pyobj = (PySpacecraftDesignObject *)PyObject_Call((PyObject *)&PySpacecraftDesignType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_spacecraftdesign(PyObject *m)
{
    if (PyType_Ready(&PySpacecraftDesignType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PySpacecraftDesignType);
    if (PyModule_AddObject(m, "SpacecraftDesign", (PyObject *)&PySpacecraftDesignType) < 0)
    {
        Py_DECREF(&PySpacecraftDesignType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
