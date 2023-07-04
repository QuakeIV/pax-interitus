#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
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
#include "componentwrapper.h"
#include "componentdesignwrapper.h"
#include "capacitordesignwrapper.h"
#include "insulatorwrapper.h"
#include "conductorwrapper.h"
#include "spacecraft/spacecraft.h"

static void type_dealloc(PySpacecraftObject *self)
{
    if (!self->tracked)
        delete self->ref;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static bool wrapper_newup = true;
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PySpacecraftObject *object = (PySpacecraftObject *)type->tp_alloc(type, 0);
    if (wrapper_newup)
    {
        PyCelestialObject *p_pytype;
        double r;
        if (!PyArg_ParseTuple(args, "O!d", &PyCelestialType, &p_pytype, &r))
            return NULL;
        Celestial *p = p_pytype->ref;
        object->ref = new Spacecraft(p,r);
    }
    object->tracked = false;
    return (PyObject*)object;
}

// attribute functions
static PyObject* get_name(PySpacecraftObject *self, void *closure)
{
    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static int set_name(PySpacecraftObject *self, PyObject *value, void *closure)
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
static PyObject* get_design(PySpacecraftObject *self, void *closure)
{
    return (PyObject*)pyobjectize_spacecraftdesign(&self->ref->design);
}
static int set_design(PySpacecraftObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PySpacecraftDesignType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign SpacecraftDesign type to Spacecraft.design.");
        return -1;
    }
    PySpacecraftDesignObject *v = (PySpacecraftDesignObject*)value;
    self->ref->design = *v->ref;
    v->tracked = true;
    return 0;
}
static PyObject* get_trajectory(PySpacecraftObject *self, void *closure)
{
    if (!self->ref->trajectory)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_orbit(self->ref->trajectory);
}
static int set_trajectory(PySpacecraftObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyOrbitType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign Orbit type to Spacecraft.trajectory.");
        return -1;
    }
    PyOrbitObject *v = (PyOrbitObject*)value;
    *self->ref->trajectory = *v->ref;
    return 0;
}
static PyObject* get_position(PySpacecraftObject *self, void *closure)
{
    return (PyObject*)pyobjectize_fixedv2d(&self->ref->position);
}
static int set_position(PySpacecraftObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyFixedV2DType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only assign FixedV2D type to Spacecraft.position.");
        return -1;
    }
    PyFixedV2DObject *v = (PyFixedV2DObject*)value;
    self->ref->position = *v->ref;
    v->tracked = true;
    return 0;
}
static PyObject* get_engines(PySpacecraftObject *self, void *closure)
{
    PyObject *engines_pylist = PyList_New(0);
    foreach (Engine *element, self->ref->engines)
    {
        PyEngineObject *py_element = pyobjectize_engine(element);
        PyList_Append(engines_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return engines_pylist;
}
static int set_engines(PySpacecraftObject *self, PyObject *value, void *closure)
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
static PyObject* get_reactors(PySpacecraftObject *self, void *closure)
{
    PyObject *reactors_pylist = PyList_New(0);
    foreach (Reactor *element, self->ref->reactors)
    {
        PyReactorObject *py_element = pyobjectize_reactor(element);
        PyList_Append(reactors_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return reactors_pylist;
}
static int set_reactors(PySpacecraftObject *self, PyObject *value, void *closure)
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
static PyObject* get_directed_weapons(PySpacecraftObject *self, void *closure)
{
    PyObject *directed_weapons_pylist = PyList_New(0);
    foreach (Directedweapon *element, self->ref->directed_weapons)
    {
        PyDirectedweaponObject *py_element = pyobjectize_directedweapon(element);
        PyList_Append(directed_weapons_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return directed_weapons_pylist;
}
static int set_directed_weapons(PySpacecraftObject *self, PyObject *value, void *closure)
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
static PyObject* get_jump_drives(PySpacecraftObject *self, void *closure)
{
    PyObject *jump_drives_pylist = PyList_New(0);
    foreach (Jumpdrive *element, self->ref->jump_drives)
    {
        PyJumpdriveObject *py_element = pyobjectize_jumpdrive(element);
        PyList_Append(jump_drives_pylist, (PyObject *)py_element);
        Py_DECREF(py_element);
    }
    return jump_drives_pylist;
}
static int set_jump_drives(PySpacecraftObject *self, PyObject *value, void *closure)
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
    "design",
    (getter)get_design,
    (setter)set_design,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "trajectory",
    (getter)get_trajectory,
    (setter)set_trajectory,
    NULL, // documentation string
    NULL, // closure
    },
    {
    "position",
    (getter)get_position,
    (setter)set_position,
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
    {NULL},
};

// wrapped function calls
static PyObject *func_ready_to_jump(PySpacecraftObject *self, PyObject *args)
{
    PyOrbitObject *tgt_pytype;
    if (!PyArg_ParseTuple(args, "O!", &PyOrbitType, &tgt_pytype))
        return NULL;
    Orbit *tgt = tgt_pytype->ref;
    return PyBool_FromLong(self->ref->ready_to_jump(tgt));
}
static PyObject *func_jump(PySpacecraftObject *self, PyObject *args)
{
    PyOrbitObject *tgt_pytype;
    if (!PyArg_ParseTuple(args, "O!", &PyOrbitType, &tgt_pytype))
        return NULL;
    Orbit *tgt = tgt_pytype->ref;
    return PyBool_FromLong(self->ref->jump(tgt));
}
static PyObject *func_select_jumpdrive(PySpacecraftObject *self, PyObject *args)
{
    PyJumpdriveObject *drive_pytype;
    if (!PyArg_ParseTuple(args, "O!", &PyJumpdriveType, &drive_pytype))
        return NULL;
    Jumpdrive *drive = drive_pytype->ref;
    return PyBool_FromLong(self->ref->select_jumpdrive(drive));
}
static PyMethodDef  methods[] = {
    {"ready_to_jump", (PyCFunction)func_ready_to_jump, METH_VARARGS, PyDoc_STR("Wraps a call to ready_to_jump.")},
    {"jump", (PyCFunction)func_jump, METH_VARARGS, PyDoc_STR("Wraps a call to jump.")},
    {"select_jumpdrive", (PyCFunction)func_select_jumpdrive, METH_VARARGS, PyDoc_STR("Wraps a call to select_jumpdrive.")},
    {NULL, NULL}
};

static PyObject* __eq__(PySpacecraftObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PySpacecraftType))
        Py_RETURN_FALSE;
    PySpacecraftObject *other_cast = (PySpacecraftObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PySpacecraftType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.Spacecraft",
    .tp_basicsize = sizeof(PySpacecraftObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython Spacecraft Type Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_methods = methods,
    .tp_getset = getsets,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PySpacecraftObject *pyobjectize_spacecraft(Spacecraft *obj)
{
    wrapper_newup = false;
    PySpacecraftObject *pyobj = (PySpacecraftObject *)PyObject_Call((PyObject *)&PySpacecraftType,PyTuple_New(0),NULL);
    pyobj->ref = obj;
    pyobj->tracked = true;
    wrapper_newup = true;
    return pyobj;
}

// Inits the type and adds it as a member to module
// On failure returns false with exception set and decrefs module
bool init_spacecraft(PyObject *m)
{
    if (PyType_Ready(&PySpacecraftType) < 0)
    {
        Py_DECREF(m);
        return false;
    }
    Py_INCREF(&PySpacecraftType);
    if (PyModule_AddObject(m, "Spacecraft", (PyObject *)&PySpacecraftType) < 0)
    {
        Py_DECREF(&PySpacecraftType);
        Py_DECREF(m);
        return false;
    }
    return true;
}
