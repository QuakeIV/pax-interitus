
#include <Python.h>
#include "universe.h"
#include "pysystemrenderer.h"
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

// TODO: set parent?
static PyObject *method_add_systemrenderer_widget(PyObject *self, PyObject *args)
{
    uint64_t ptr = 0;
    // Parse arguments
    if(!PyArg_ParseTuple(args, "K", &ptr))
        return Py_False;
    
    QLayout *layout = (QLayout *)ptr;
    PySystemRenderer *renderer = new PySystemRenderer();
    layout->addWidget(renderer);
    return Py_True;
}

// currently does nothing
static PyObject *method_test(PyObject *self, PyObject *args)
{
//    uint64_t ptr = 0;
    // Parse arguments
    //PyObject *obj;
    //if(!PyArg_ParseTuple(args, "O", &obj))
    //    return Py_False;
    uint64_t ptr = 0;
    if(!PyArg_ParseTuple(args, "K", &ptr))
        return Py_False;
    QMainWindow *mw = (QMainWindow *)ptr;
    PySystemRenderer *renderer = new PySystemRenderer(mw);
    mw->setCentralWidget(renderer);
//    void* ptr = obj;
    
//    printf("%p\n", obj);
//    printf("%p\n", ptr);
    
    //QVBoxLayout *layout = (QVBoxLayout *)ptr;
//    QWidget *parent = new QWidget();
    //SystemRenderer *renderer = new SystemRenderer(0);
    //layout->addWidget(renderer);

    return Py_True;
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
    {"addSystemRenderer", method_add_systemrenderer_widget, METH_VARARGS, "Takes layout as argument, adds system renderer widget to the layout."},
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
    universe_init();
    return PyModule_Create(&libpaxpythonmodule);
}
