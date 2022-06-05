#include <Python.h>
#include "systemrenderer.h"
#include "universe.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>

extern QApplication *qapp;
static PyTypeObject **SbkPySide6_QtWidgetsTypes;

static PyObject *method_bind_to_qapp(PyObject *self, PyObject *args)
{
    //TODO: temp
    universe_init();
    
    uint64_t ptr = 0;
    // Parse arguments
    if(!PyArg_ParseTuple(args, "K", &ptr))
        return Py_False;
    
    //TODO: make it clear there is not currently error checkign on this
    qapp = (QApplication *)ptr;
    
    return Py_True;
}

static PyObject *method_add_systemrenderer_to_layout(PyObject *self, PyObject *args)
{
    uint64_t ptr = 0;
    // Parse arguments
    if(!PyArg_ParseTuple(args, "K", &ptr))
        return Py_False;
    
    QVBoxLayout *layout = (QVBoxLayout *)ptr;
    printf("%p\n",(void*)ptr);
//    QWidget *parent = new QWidget();
    SystemRenderer *renderer = new SystemRenderer(0);
    layout->addWidget(renderer);

    return Py_True;
}

static PyObject *method_test(PyObject *self, PyObject *args)
{
//    uint64_t ptr = 0;
    // Parse arguments
    PyObject *obj;
    if(!PyArg_ParseTuple(args, "O", obj))
        return Py_False;
    
    void* ptr = obj;
    
    printf("%p\n", obj);
    printf("%p\n", ptr);
    
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

static PyMethodDef paxpythonMethods[] = {
//TODO: for the love of toast try to find some other way to do the keyboard stuff
    {"bindToQApp", method_bind_to_qapp, METH_VARARGS, "Pax Interitus requires a QApplication pointer for keyboard logic to work"},
    {"addSystemRendererToLayout", method_add_systemrenderer_to_layout, METH_VARARGS, "Python interface for pax interitus system renderer widget"},
    {"test", method_test, METH_VARARGS, "generic function to play around with"},
    {NULL, NULL, 0, NULL} // i think this just terminates the list
};


static struct PyModuleDef paxpythonmodule = {
    PyModuleDef_HEAD_INIT,
    "paxpython",
    "Python interface for pax interitus",
    -1,
    paxpythonMethods
};

PyMODINIT_FUNC PyInit_paxpython(void)
{
//    Shiboken::AutoDecRef requiredModule(Shiboken::Module::import("PySide6.QtCore"));
//    SbkPySide6_QtWidgetsTypes = Shiboken::Module::getTypes(requiredModule);
    return PyModule_Create(&paxpythonmodule);
}
