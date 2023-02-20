#include <Python.h>
#include <structmember.h> // additional python context (forgot what exactly)
#include "pywrappers.h"
#include "universe.h"
#include <QList>

static void type_dealloc(PyQListObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    return type->tp_alloc(type, 0);
}

// wrapped function calls

static PyObject* __eq__(PyQListObject *self, PyObject *other, int op)
{
    if (!PyObject_IsInstance(other, (PyObject *)&PyQListType))
        Py_RETURN_FALSE;
    PyQListObject *other_cast = (PyQListObject *)other;
    if (op == Py_EQ && self->ref == other_cast->ref)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static Py_ssize_t size(PyQListObject *self)
{
    return self->ref->length();
}

// TODO: we want to hold the lock while iterating until done doing that, honestly
// maybe we can somehow hand over an iterator object that will do that
// for now, whatever
static PyObject *getitem(PyQListObject *self, Py_ssize_t i)
{
    if (i < self->ref->length())
    {
        universe_lock.lock_shared();
        PyObject *obj = self->objectize_func((*self->ref)[i]);
        universe_lock.unlock_shared();
        return obj;
    }
    return NULL;
}

// just providing a presumably somewhat faster 'contains' rather than having python traverse the whole list
static int contains(PyQListObject *self, PyObject *value)
{
    if (PyObject_IsInstance(value, (PyObject *)self->type))
    {
        universe_lock.lock_shared();
        PyPaxgenericObject *obj = (PyPaxgenericObject*)value;
        bool match = self->ref->contains(obj->ref);
        universe_lock.unlock_shared();
        if (match)
            return 1;
    }
    return 0;
}

// assign item (handles deletion only at this point, assignment is unclear with respect to things like the .tracked attribute)
// TODO: actually maybe thats straightforward and we just let the object get garbage collected naturally?
// actually we might want to SET that attribute ourselves as this is assumed to be accessing universe state
static int assitem(PyQListObject *self, Py_ssize_t i, PyObject *v)
{
    // only support deletion at this point
    if (v == NULL && i < self->ref->length())
    {
        universe_lock.lock();
        self->ref->removeAt(i);
        universe_lock.unlock();
        return 0;
    }
    return -1;
}

static PyObject *type_append(PyQListObject *self, PyObject *args)
{
    PyPaxgenericObject *o;
    if (PyArg_ParseTuple(args, "O!", self->type, &o))
    {
        self->ref->append(o->ref); 
        o->tracked = true;
        Py_RETURN_NONE; // success
    }
    return NULL; // failure (exception)
}


// TODO: may be appropriate to add removeall as well (i think this should stay as the python-equivalent to list.remove however)
static PyObject *type_remove(PyQListObject *self, PyObject *args)
{
    PyPaxgenericObject *o;
    if (PyArg_ParseTuple(args, "O!", self->type, &o))
    {
        if (self->ref->removeOne(o->ref))
        {
            // TODO: this is really not ideal, basically forces everyone to treat these objects as ephemeral in case someone else deletes it out from under you
            // could potentially make this work with a ref counter if the python object is eh... dont remember the term but if you are always returned the same wrapper object
            // could facilitate that with a 'wrapper' pointer on the actual base object?
            // if we can then find a solution for whether the object is tracked or not on deletion (ie what lists it might be registered in) we are good to go
            o->tracked = false;
            Py_RETURN_NONE; // success
        }
        else
        {
            PyErr_SetString(PyExc_ValueError, "Value not found."); // using return NULL below
        }
    }
    return NULL; // failure (exception)
}

static PyMethodDef methods[] = {
    {"append", (PyCFunction)type_append, METH_VARARGS, PyDoc_STR("Appends to the list.")},
    {"remove", (PyCFunction)type_remove, METH_VARARGS, PyDoc_STR("Removes first matching element from list.")},
    {NULL, NULL}
};

static PySequenceMethods seqmethods = {
    .sq_length = (lenfunc)size,
    .sq_item = (ssizeargfunc)getitem,
    .sq_ass_item = (ssizeobjargproc)assitem,
    .sq_contains = (objobjproc)contains,
};

PyTypeObject PyQListType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.QList",
    .tp_basicsize = sizeof(PyQListObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_as_sequence = &seqmethods,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("PaxPython QList Wrapper."),
    .tp_richcompare = (richcmpfunc)&__eq__,
    .tp_methods = methods,
    .tp_new = type_new,
};

// Takes pointer to object, returns python wrapper for object with ref count of 1
PyQListObject *pyobjectize_qlist(QList<void*> *ref, PyTypeObject *type, pyobjectize_generic_func objectize_func)
{
    // TODO: validate type? meh, can use PyType_Check(type) if so
    PyQListObject *pyobj = (PyQListObject *)PyObject_Call((PyObject *)&PyQListType,PyTuple_New(0),NULL);
    if (!pyobj)
        return NULL;
    pyobj->ref = ref;
    pyobj->type = type;
    pyobj->objectize_func = objectize_func;
    return pyobj;
}
