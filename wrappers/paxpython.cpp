
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
    // this is the actual sim, not directly python related
    universe_init();

    // NOTE: this appears to be absolutely not optional, the type will not be properly initialized without this
    if (PyType_Ready(&PyQListType) < 0) // putting this first since other types might use it
        return NULL;
    if (PyType_Ready(&PySystemRendererType) < 0)
        return NULL;

    PyObject *m = PyModule_Create(&libpaxpythonmodule);

    // hand-written universe wrapper (notably, adds instance of universe wrapper to module, rather than type)
    if (!init_universewrapper(m))
        return NULL;

    // does pytype_ready and also addobject
    if (!init_component(m))
        return NULL;
    if (!init_componentdesign(m))
        return NULL;
    if (!init_capacitor(m))
        return NULL;
    if (!init_capacitordesign(m))
        return NULL;
    if (!init_celestial(m))
        return NULL;
    if (!init_circuit(m))
        return NULL;
    if (!init_circuitdesign(m))
        return NULL;
    if (!init_conductor(m))
        return NULL;
    if (!init_directedweapon(m))
        return NULL;
    if (!init_directedweapondesign(m))
        return NULL;
    if (!init_engine(m))
        return NULL;
    if (!init_enginedesign(m))
        return NULL;
    if (!init_fixedv2d(m))
        return NULL;
    if (!init_insulator(m))
        return NULL;
    if (!init_jumpdrive(m))
        return NULL;
    if (!init_jumpdrivedesign(m))
        return NULL;
    if (!init_reactor(m))
        return NULL;
    if (!init_reactordesign(m))
        return NULL;
    if (!init_solarsystem(m))
        return NULL;
    if (!init_spacecraft(m))
        return NULL;
    if (!init_spacecraftdesign(m))
        return NULL;
    if (!init_transform(m))
        return NULL;
    if (!init_orbittype(m)) // TODO: rename this type to just be orbit
        return NULL;

    Py_INCREF(&PySystemRendererType);
    if (PyModule_AddObject(m, "SystemRenderer", (PyObject *)&PySystemRendererType) < 0)
    {
        Py_DECREF(&PySystemRendererType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
