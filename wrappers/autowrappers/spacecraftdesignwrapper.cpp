#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "units.h" // conversion factors and so on
#include "spacecraftdesignwrapper.h"
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
        object->ref = new SpacecraftDesign();
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
    "radius",
    (getter)get_radius,
    (setter)set_radius,
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
