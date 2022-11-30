#include <Python.h>
#include <structmember.h>
#include "pywrappers.h"
#include "solarsystemtype.h"

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
    // TODO: these may be pointless/wasteful paranoia?
    if (!self->ref)
        PyErr_SetString(PyExc_ValueError, "ref not initialized, error in construction of celestial type wrapper");

    return PyUnicode_FromString(self->ref->name.toStdString().c_str());
}
static PyObject* mytype_get_x(PyCelestialObject* self, void* closure)
{
    if (!self->ref)
        PyErr_SetString(PyExc_ValueError, "ref not initialized, error in construction of celestial type wrapper");

    return PyFloat_FromDouble(DISTANCE_FIXED_TO_FLOAT(self->ref->trajectory.position.x));
}
static PyObject* mytype_get_y(PyCelestialObject* self, void* closure)
{
    if (!self->ref)
        PyErr_SetString(PyExc_ValueError, "ref not initialized, error in construction of celestial type wrapper");

    return PyFloat_FromDouble(DISTANCE_FIXED_TO_FLOAT(self->ref->trajectory.position.y));
}
static PyObject* mytype_get_mass(PyCelestialObject* self, void* closure)
{
    if (!self->ref)
        PyErr_SetString(PyExc_ValueError, "ref not initialized, error in construction of celestial type wrapper");

    return PyLong_FromLongLong(self->ref->mass);
}
static PyObject* mytype_get_radius(PyCelestialObject* self, void* closure)
{
    if (!self->ref)
        PyErr_SetString(PyExc_ValueError, "ref not initialized, error in construction of celestial type wrapper");

    // cheeky manual divide by 1000 to get to meters
    // TODO: actual units.h defines for that
    return PyFloat_FromDouble(DISTANCE_FIXED_TO_FLOAT(self->ref->radius / 1000.0));
}
static PyObject* mytype_get_orbital_radius(PyCelestialObject* self, void* closure)
{
    if (!self->ref)
        PyErr_SetString(PyExc_ValueError, "ref not initialized, error in construction of celestial type wrapper");

    return PyFloat_FromDouble(self->ref->trajectory.orbital_radius);
}
static PyObject* mytype_get_orbital_period(PyCelestialObject* self, void* closure)
{
    if (!self->ref)
        PyErr_SetString(PyExc_ValueError, "ref not initialized, error in construction of celestial type wrapper");

    // TODO: pretty ineffecient, may need work eventually
    return PyUnicode_FromString(get_time_str(self->ref->trajectory.orbital_period).toStdString().c_str());
}
static PyObject* mytype_get_parent(PyCelestialObject* self, void* closure)
{
    if (!self->ref)
        PyErr_SetString(PyExc_ValueError, "ref not initialized, error in construction of celestial type wrapper");

    if (!self->ref->parent)
        Py_RETURN_NONE;

    PyCelestialObject *new_cel = (PyCelestialObject *)PyObject_Call((PyObject *)&PyCelestialType,PyTuple_New(0),NULL);
    new_cel->ref = self->ref->parent;

    return (PyObject*)new_cel;
}
static PyObject* mytype_get_children(PyCelestialObject* self, void* closure)
{
    if (!self->ref)
        PyErr_SetString(PyExc_ValueError, "ref not initialized, error in construction of celestial type wrapper");

    // TODO: this list could probably be cached in some way, at least insofar as planets are static
    PyObject *child_list = PyList_New(0);
    foreach (Celestial *c, self->ref->children)
    {
        PyCelestialObject *child = (PyCelestialObject *)PyObject_Call((PyObject *)&PyCelestialType,PyTuple_New(0),NULL);
        child->ref = c;
        PyList_Append(child_list, (PyObject *)child);
        Py_DECREF(child); // decref to undo incref from the newly created object, now that list is holding onto it
    }

    // implicitly returns empty list if no children (based)
    return (PyObject*)child_list;
}
// establish equality with other instances of wrapper (or self, for that matter)
static PyObject* __eq__(PyCelestialObject *self, PyObject *other, int op)
{
    if (!self->ref)
        PyErr_SetString(PyExc_ValueError, "ref not initialized, error in construction of celestial type wrapper");

    // only compares with same type
    if (!PyObject_IsInstance(other, (PyObject *)&PyCelestialType))
        Py_RETURN_FALSE;

    PyCelestialObject *other_cel = (PyCelestialObject *)other;
    if (op == Py_EQ && self->ref == other_cel->ref)
        Py_RETURN_TRUE;

    // if we get this far, then no
    Py_RETURN_FALSE;
}
// string representation of self
PyObject *__repr__(PyCelestialObject *self)
{
    if (!self->ref)
        PyErr_SetString(PyExc_ValueError, "ref not initialized, error in construction of celestial type wrapper");

    // this feels unbelievably non performant and wasteful, oh well (TODO: maybe snprintf or something to build this)
    QString str = "<Celestial object " + self->ref->name + " (" + self->ref->system->name + " System)>";
    return PyUnicode_FromString(str.toStdString().c_str());
}
static PyGetSetDef getsets[] = {
    {"name",
     (getter)mytype_get_name,
     NULL, //(setter) mytype_set_field
     "name of celestial",  /* doc */
     NULL /* closure */},
    {"x",
     (getter)mytype_get_x,
     NULL, //(setter) mytype_set_field
     "x coordinate of celestial",  /* doc */
     NULL /* closure */},
    {"y",
     (getter)mytype_get_y,
     NULL, //(setter) mytype_set_field
     "y coordinate of celestial",  /* doc */
     NULL /* closure */},
    {"mass",
     (getter)mytype_get_mass,
     NULL, //(setter) mytype_set_field
     "mass of celestial",  /* doc */
     NULL /* closure */},
    {"radius",
     (getter)mytype_get_radius,
     NULL, //(setter) mytype_set_field
     "radius of celestial",  /* doc */
     NULL /* closure */},
    {"orbital_radius",
     (getter)mytype_get_orbital_radius,
     NULL, //(setter) mytype_set_field
     "orbital radius of celestial",  /* doc */
     NULL /* closure */},
    {"orbital_period",
     (getter)mytype_get_orbital_period,
     NULL, //(setter) mytype_set_field
     "orbital period of celestial",  /* doc */
     NULL /* closure */},
    {"parent",
     (getter)mytype_get_parent,
     NULL, //(setter) mytype_set_field
     "parent body of celestial",  /* doc */
     NULL /* closure */},
    {"children",
     (getter)mytype_get_children,
     NULL, //(setter) mytype_set_field
     "children of celestial",  /* doc */
     NULL /* closure */},
    {NULL}
};
// TODO: maybe the actual celestial objects should keep a cache of python wrappers for themselves?
// this also kindof relates to whether or not everything in the universe should become a python object as well
PyTypeObject PyCelestialType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.Celestial",
    .tp_basicsize = sizeof(PyCelestialObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_repr = (reprfunc)__repr__,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = PyDoc_STR("PaxPython Celestial Type Wrapper"),
    .tp_richcompare = (richcmpfunc)&__eq__,
    //.tp_methods = Custom_methods,
    //.tp_members = Custom_members,
    //.tp_init = (initproc)Custom_init,
    .tp_getset = getsets,
    .tp_new = type_new,
};
