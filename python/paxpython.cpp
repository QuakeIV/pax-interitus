
#include <Python.h>
#include <structmember.h>
#include "universe.h"
#include "pysystemrenderer.h"
#include "pywrappers.h"
#include <QLayout>
#include <QMainWindow>

static PyObject *method_test(PyObject *self, PyObject *args)
{
    Py_RETURN_TRUE;
}

/*
static PyObject *method_fputs(PyObject *self, PyObject *args)
{
    char *str, *filename = NULL;
    int bytes_copied = -1;
    // Parse arguments
    if(!PyArg_ParseTuple(args, "ss", &str, &filename)) {
        return NULL;
    }
    FILE *fp = fopen(filename, "w");
    bytes_copied = fputs(str, fp);
    fclose(fp);
    return PyLong_FromLong(bytes_copied);
}*/

static PyMethodDef libpaxpythonMethods[] = {
    {"test", method_test, METH_VARARGS, "generic function to play around with"},
    {NULL, NULL, 0, NULL} // i think this just terminates the list
};


static struct PyModuleDef libpaxpythonmodule = {
    PyModuleDef_HEAD_INIT,
    "paxpython",
    "Python interface for pax interitus",
    -1,
    libpaxpythonMethods
};

PyMODINIT_FUNC PyInit_libpaxpython(void)
{
    // NOTE: this appears to be absolutely not optional, the type will not be properly initialized without this
    if (PyType_Ready(&PySystemRendererType) < 0)
        return NULL;
    if (PyType_Ready(&PyUniverseType) < 0)
        return NULL;

    PyObject *m = PyModule_Create(&libpaxpythonmodule);

    // does pytype_ready and also addobject
    if (!init_celestial(m))
        return NULL;
    if (!init_insulator(m))
        return NULL;
    if (!init_capacitordesign(m))
        return NULL;
    if (!init_capacitor(m))
        return NULL;
    if (!init_orbittype(m)) // TODO: rename this type to just be orbit
        return NULL;
    if (!init_fixedv2d(m))
        return NULL;
    if (!init_spacecraftdesign(m))
        return NULL;
    if (!init_spacecraft(m))
        return NULL;

    Py_INCREF(&PySystemRendererType);
    if (PyModule_AddObject(m, "SystemRenderer", (PyObject *)&PySystemRendererType) < 0)
    {
        Py_DECREF(&PySystemRendererType);
        Py_DECREF(m);
        return NULL;
    }

    // different from others, this is instantiating a generic object and adding it to module, rather than adding a type object
    PyObject *u = PyObject_Call((PyObject *)&PyUniverseType,PyTuple_New(0),NULL);
    if (PyModule_AddObject(m, "universe", u) < 0)
    {
        Py_DECREF(u);
        Py_DECREF(m);
        return NULL;
    }

    // this is the actual sim, not python related
    universe_init();

    return m;
}
