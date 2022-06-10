#include <Python.h>
#include "systemrenderer.h"
#include "universe.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>

extern QApplication *qapp;
extern QWidget *main_widget;

static PyObject *method_bind_to_qapp(PyObject *self, PyObject *args)
{
    uint64_t ptr = 0;
    // Parse arguments
    if(!PyArg_ParseTuple(args, "K", &ptr))
        return Py_False;
    
    qapp = (QApplication *)ptr;
    
    return Py_True;
}

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


static PyObject *method_define_main_widget(PyObject *self, PyObject *args)
{
    uint64_t ptr = 0;
    // Parse arguments
    if(!PyArg_ParseTuple(args, "K", &ptr))
        return Py_False;
    
    main_widget = (QWidget *)ptr;
    
    return Py_True;
}

// TODO: set parent?
static PyObject *method_add_systemrenderer_to_layout(PyObject *self, PyObject *args)
{
    uint64_t ptr = 0;
    // Parse arguments
    if(!PyArg_ParseTuple(args, "K", &ptr))
        return Py_False;
    
    QVBoxLayout *layout = (QVBoxLayout *)ptr;
//    printf("%p\n",(void*)ptr);
    SystemRenderer *renderer = new SystemRenderer(main_widget);
    layout->addWidget(renderer);

    return Py_True;
}

// currently does nothing
static PyObject *method_test(PyObject *self, PyObject *args)
{
//    uint64_t ptr = 0;
    // Parse arguments
    PyObject *obj;
    if(!PyArg_ParseTuple(args, "O", obj))
        return Py_False;
    
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
//TODO: for the love of toast try to find some other way to do the keyboard stuff
    {"bindToQApp", method_bind_to_qapp, METH_VARARGS, "Pax Interitus requires a QApplication pointer for keyboard logic to work"},
    {"defineMainWidget", method_define_main_widget, METH_VARARGS, "Allows you to set the main parent window which will cause all others to close when it does"},
    {"addSystemRendererToLayout", method_add_systemrenderer_to_layout, METH_VARARGS, "Python interface for pax interitus system renderer widget"},
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
