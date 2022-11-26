
#include <Python.h>
#include <structmember.h>
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

// Python Type
//static PyMethodDef Custom_methods[] = {
//    {"name", (PyCFunction) Custom_name, METH_NOARGS,
//     "Return the name, combining the first and last name"
//    },
//    {NULL}  /* terminate */
//};
static PyMemberDef SystemRendererPyType_members[] = {
    // for now, singleclick is basically only for refocusing, so we wont call back into python for it
    //{"single_click_callback", T_OBJECT, offsetof(PySystemRendererObject, singleClickCallback), 0,
    // "If set to a callable, is called whenever the system renderer registers a left click.  Effected game objects passed."},
    {"right_click_callback", T_OBJECT, offsetof(PySystemRendererObject, rightClickCallback), 0,
     "If set to a callable, is called whenever the system renderer registers a right click.  Effected game objects passed"},
    {"double_click_callback", T_OBJECT, offsetof(PySystemRendererObject, doubleClickCallback), 0,
     "If set to a callable, is called whenever the system renderer registers a right click.  Effected game objects passed"},
    {NULL}  /* terminate */
};
static void SystemRendererPyType_dealloc(PySystemRendererObject *self)
{
    Py_XDECREF(self->singleClickCallback);
    Py_XDECREF(self->rightClickCallback);
    Py_XDECREF(self->doubleClickCallback);
    delete self->renderer;
    Py_TYPE(self)->tp_free((PyObject *) self);
}
static PyObject *SystemRendererPyType_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    uint64_t ptr = 0;
    // Parse arguments
    if(!PyArg_ParseTuple(args, "K", &ptr))
        return NULL;

    PySystemRendererObject *self;
    self = (PySystemRendererObject *) type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->singleClickCallback = NULL;
        self->rightClickCallback  = NULL;
        self->doubleClickCallback = NULL;

        QLayout *layout = (QLayout *)ptr;
        PySystemRenderer *renderer = new PySystemRenderer(layout->parentWidget());
        layout->addWidget(renderer);
        renderer->py_obj = self;
        self->renderer = renderer;
    }
    return (PyObject *) self;
}
static PyTypeObject SystemRendererPyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.SystemRenderer",
    .tp_basicsize = sizeof(PySystemRendererObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) SystemRendererPyType_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = PyDoc_STR("PaxPython Solar System Renderer"),
    //.tp_methods = Custom_methods,
    .tp_members = SystemRendererPyType_members,
    //.tp_init = (initproc) Custom_init,
    .tp_new = SystemRendererPyType_new,
};

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
    if (PyType_Ready(&SystemRendererPyType) < 0)
        return NULL;

    PyObject *m = PyModule_Create(&libpaxpythonmodule);

    Py_INCREF(&SystemRendererPyType);
    if (PyModule_AddObject(m, "SystemRenderer", (PyObject *)&SystemRendererPyType) < 0)
    {
        Py_DECREF(&SystemRendererPyType);
        Py_DECREF(m);
        return NULL;
    }

    universe_init();

    return m;
}
