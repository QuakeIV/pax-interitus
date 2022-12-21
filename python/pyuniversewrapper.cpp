#include <Python.h>
#include <structmember.h>
#include "pywrappers.h"
#include "universe.h"

static void type_dealloc(PySpacecraftObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *) self);
}
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    return type->tp_alloc(type, 0);
}
static PyObject* get_min_warp(PySpacecraftObject* self, void* closure)
{
    // TODO: this is effectively const, we could hold this number and then incref/return it instead of constructing a new one repeatedly
    return PyLong_FromLongLong(universe_min_warp);
}
static PyObject* get_max_warp(PySpacecraftObject* self, void* closure)
{
    // TODO: this is effectively const, we could hold this number and then incref/return it instead of constructing a new one repeatedly
    return PyLong_FromLongLong(universe_max_warp);
}
static PyObject* get_time_warp(PySpacecraftObject* self, void* closure)
{
    return PyLong_FromLongLong(universe_time_warp);
}
static int set_time_warp(PySpacecraftObject* self, PyObject *value, void *closure)
{
    int64_t val = PyLong_AsLongLong(value);

    if (PyErr_Occurred())
        return -1; // exception

    if (val < universe_min_warp)
        universe_time_warp = universe_min_warp;
    else if (val > universe_max_warp)
        universe_time_warp = universe_max_warp;
    else
        universe_time_warp = val;

    return 0;
}
static PyObject* get_paused(PySpacecraftObject* self, void* closure)
{
    return PyBool_FromLong(universe_paused);
}
static int set_paused(PySpacecraftObject* self, PyObject *value, void *closure)
{
    universe_paused = PyObject_IsTrue(value);
    return 0;
}
static PyGetSetDef getsets[] = {
    {"time_warp",
     (getter)get_time_warp,
     (setter)set_time_warp,
     "time warp factor (exponent term)",  /* doc */
     NULL /* closure */},
    {"min_warp",
     (getter)get_min_warp,
     NULL,
     "min time warp factor (exponent term)",  /* doc */
     NULL /* closure */},
    {"max_warp",
     (getter)get_max_warp,
     NULL,
     "max time warp factor (exponent term)",  /* doc */
     NULL /* closure */},
    {"paused",
     (getter)get_paused,
     (setter)set_paused,
     "whether universe is paused or not",  /* doc */
     NULL /* closure */},
    {NULL}
};
PyTypeObject PyUniverseType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.Universe",
    .tp_basicsize = sizeof(PyObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = PyDoc_STR("PaxPython Universe Wrapper"),
    //.tp_methods = Custom_methods,
    //.tp_members = Custom_members,
    //.tp_init = (initproc)Custom_init,
    .tp_getset = getsets,
    .tp_new = type_new,
};
