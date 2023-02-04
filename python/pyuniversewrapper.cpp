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
// TODO: for this functionality, it would i think be more performant to keep a running list that can be referenced, rather than re-generating every time
// can have a function that is called every frame or something that is just update_universe and it coherency-izes all the universe data structures in one big pass
// maybe a callback infrastructure to track changes to the primary lists, i think we were adding stuff to lists in their constructor, so that could potentially
// serve as a place to put a callback (if we dont drop the idea of C++ ultimately altogether)
static PyObject* get_insulators(PySpacecraftObject* self, void* closure)
{
    // TODO: might not technically need this
    universe_lock.lock_shared();
    PyObject *insulator_list = PyList_New(0);
    foreach (Insulator *i, insulator_materials)
    {
        PyInsulatorObject *insulator = pyobjectize_insulator(i);
        PyList_Append(insulator_list, (PyObject *)insulator);
        Py_DECREF(insulator); // decref to undo incref from the newly created object, now that list is holding onto it
    }
    universe_lock.unlock_shared();
    return insulator_list;
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
    {"insulators",
     (getter)get_insulators,
     NULL,
     "list of all insulators in the universe",  /* doc */
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
