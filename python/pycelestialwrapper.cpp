#include <Python.h>
#include <structmember.h>
#include "pywrappers.h"

static void type_dealloc(PyCelestialObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *) self);
}
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    return type->tp_alloc(type, 0);
}
static PyObject* mytype_get_name(PyCelestialObject* self, void* closure)
{
    if (!self->ref)
        PyErr_SetString(PyExc_ValueError, "ref not initialized, error in construction of celestial type wrapper");

    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static PyGetSetDef getsets[] = {
    {"name",
     (getter)mytype_get_name,
     NULL, //(setter) mytype_set_field
     "name of celestial",  /* doc */
     NULL /* closure */},
    {NULL}
};
PyTypeObject PyCelestialType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.Celestial",
    .tp_basicsize = sizeof(PyCelestialObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = PyDoc_STR("PaxPython Celestial Type Wrapper"),
    //.tp_methods = Custom_methods,
    //.tp_members = Custom_members,
    //.tp_init = (initproc)Custom_init,
    .tp_getset = getsets,
    .tp_new = type_new,
};
