#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "spacecraftwrapper.h"
#include "spacecraftdesignwrapper.h"
#include "transformwrapper.h"
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
        object->ref = new Spacecraft();
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
        PyErr_SetString(PyExc_TypeError, "Can only set value to SpacecraftDesign.");
        return -1;
    }
    PySpacecraftDesignObject *v = (PySpacecraftDesignObject*)value;
    self->ref->design = *v->ref;
    return 0;
}
static PyObject* get_trajectory(PySpacecraftObject *self, void *closure)
{
    if (!self->ref->trajectory)
        Py_RETURN_NONE;
    return (PyObject*)pyobjectize_transform(self->ref->trajectory);
}
static int set_trajectory(PySpacecraftObject *self, PyObject *value, void *closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute.");
        return -1;
    }
    if (!PyObject_IsInstance(value, (PyObject *)&PyTransformType))
    {
        PyErr_SetString(PyExc_TypeError, "Can only set value to Transform.");
        return -1;
    }
    PyTransformObject *v = (PyTransformObject*)value;
    self->ref->trajectory = v->ref;
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
    {NULL},
};

// wrapped function calls

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
