
#include <Python.h>
#include <structmember.h>
#include "universe.h"
#include "pysystemrenderer.h"
#include "pywrappers.h"
#include <QLayout>
#include <QMainWindow>

// handy snippet for later
//extern "C"
//{
//    PyObject* foo(const char* FILE_NAME)
//    {
//        string line;
//        ifstream myfile(FILE_NAME);
//        PyObject* result = PyList_New(0);
//
//        while (getline(myfile, line))
//        {
//            PyList_Append(result, PyLong_FromLong(1));
//        }
//
//        return result;
//    }
//}

// currently does nothing
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
    if (PyType_Ready(&PyCelestialType) < 0)
        return NULL;

    PyObject *m = PyModule_Create(&libpaxpythonmodule);

    Py_INCREF(&PySystemRendererType);
    if (PyModule_AddObject(m, "SystemRenderer", (PyObject *)&PySystemRendererType) < 0)
    {
        Py_DECREF(&PySystemRendererType);
        Py_DECREF(m);
        return NULL;
    }

    universe_init();

    return m;
}
