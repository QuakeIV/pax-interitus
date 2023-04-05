

class FileWriter:
  def __init__(self, filename):
    self.file = open(filename, "w")
    self._indentation = []
  #
  
  def close(self):
    self.file.close()
  #
  
  def indent(self, indent="    "):
    self._indentation.append(indent)
  #
  
  def dedent(self):
    self._indentation.pop()
  #
  
  def raw(self, s="", end=""):
    self.file.write(s + end)
    print(s, end="")
  #
  
  def write(self, s="", end="\n"):
    self.file.write("".join(self._indentation) + s + end)
  #
#

def unknown_attr_check(thing):
  if thing:
    raise KeyError(f"unrecognized keys for file ({args.i}): {list(thing.keys())}")
#
  


# json5 allows trailing commas
import sys, json5, os, argparse

parser = argparse.ArgumentParser(
  prog = "Paxpython Type Wrapper Generator",
  description= "Generates python objects wrapping pax interitus types.  Will instantiate the type and retain a pointer to it.  Autogenned python wrapper will deallocate the type on destruction if it was not tracked by the game state in some way.",
  epilog = "Bottom text lol"
)
parser.add_argument("-o", metavar="output directory", default=".")
required = parser.add_argument_group('required arguments')
required.add_argument("-i", metavar="input config json path",  required=True)
args = parser.parse_args()

# set default values, leave out non-optional values so there are errors downstream
with open(args.i) as f:
  cfg = json5.load(f)
#

# TOOD: it might be better to just derive this from the file name, however the question of capitalization does then come up since most of the types are like This
object_type = cfg.pop("type")

# this is used to declare structured subtypes that are also wrapped in python objects
# this drives header includes and type validation
subtypes = cfg.pop("subtypes") if "subtypes" in cfg else []

# this means the type cannot be instantiated from python
instantiation_disabled = cfg.pop("disable_instantiation") if "disable_instantiation" in cfg else False

attrs = []
if "attrs" in cfg:
  for r in cfg.pop("attrs"):
    v = {}
    v["name"] = r.pop("name")
    t = r.pop("type")
    t = t.split("*")
    v["type"] = t[0].strip()
    tt = r.pop("template_type") if "template_type" in r else ""
    tt = tt.split("*")
    v["template_type"] = tt[0].strip() if tt[0] else None
    v["template_type_ptr"] = len(tt) > 1
    v["readonly"] = r.pop("readonly") if "readonly" in r else False
    v["ptr"] = len(t) > 1 # if there is a * assume its a pointer
    if v["type"] not in ["QString", "QList", "bool", "double", "celestialmass", "fixeddistance", "fixedtime", "uint64_t"] + subtypes + [object_type]:
      raise TypeError(f"Unrecognized type: {t[0].strip()}")
    if v["type"] == "QList":
      if v["template_type"] not in [] + subtypes + [object_type]:
        raise TypeError(f"Template type {v['template_type']} not handled for QList")
      if not v["template_type_ptr"]:
        raise TypeError(f"Template type {v['template_type']} must be pointer for QList")
    unknown_attr_check(r)
    attrs.append(v)
  #
#

funcs = []
if "funcs" in cfg:
  for r in cfg.pop("funcs"):
    f = {}
    f["name"] = r.pop("name")
    f["type"] = r.pop("type")
    if f["type"] not in ["double", "fixedtime", "fixedenergy"]:
      raise TypeError(f"Unrecognized return type: {f['type']}")
    f["args"] = []
    for a in r.pop("args"):
      arg = {}
      arg["name"] = a.pop("name")
      arg["type"] = a.pop("type")
      if arg["type"] not in ["double"]:
        raise TypeError(f"Unrecognized arg type: {f['type']}")
      f["args"].append(arg)
    funcs.append(f)
#

type_header = cfg.pop("header")

# error on unrecognized keys
unknown_attr_check(cfg)



# start file writing

header = FileWriter(os.path.join(args.o, f"{object_type.lower()}wrapper.h"))

header.write(f"#ifndef PYWRAPPER_{object_type.upper()}_H")
header.write(f"#define PYWRAPPER_{object_type.upper()}_H")
header.write()
header.write("#include <Python.h>")
header.write()
header.write(f"class {object_type};")
header.write("typedef struct")
header.write("{")
header.indent()
header.write("PyObject_HEAD")
header.write(f"{object_type} *ref;")
header.write("bool tracked;")
header.dedent()
header.write(f"}} Py{object_type}Object;")
header.write()
header.write("// Inits the type and adds it as a member to module")
header.write("// On failure returns false with exception set and decrefs module")
header.write(f"bool init_{object_type.lower()}(PyObject *m);")
header.write()
header.write("// Takes pointer to object, returns python wrapper for object with ref count of 1")
header.write(f"Py{object_type}Object *pyobjectize_{object_type.lower()}({object_type} *obj);")
header.write()
header.write(f"extern PyTypeObject Py{object_type}Type;")
header.write()
header.write(f"#endif // PYWRAPPER_{object_type.upper()}_H")
header.close()

source = FileWriter(os.path.join(args.o, f"{object_type.lower()}wrapper.cpp"))

source.write("#include <Python.h>")
source.write("#include <structmember.h> // additional python context (forgot what exactly)")
source.write("#include \"units.h\" // conversion factors and so on")
source.write(f"#include \"{object_type.lower()}wrapper.h\"")
for s in subtypes:
  source.write(f"#include \"{s.lower()}wrapper.h\"")
source.write(f"#include \"{type_header}\"")
source.write()
source.write(f"static void type_dealloc(Py{object_type}Object *self)")
source.write("{")
source.indent()
source.write("if (!self->tracked)")
source.indent()
source.write("delete self->ref;")
source.dedent()
source.write("Py_TYPE(self)->tp_free((PyObject *)self);")
source.dedent()
source.write("}")
source.write()
source.write("static bool wrapper_newup = true;")
source.write("static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)")
source.write("{")
source.indent()
if instantiation_disabled:
  # TODO: currently on 3.8, there are far better ways to handle this in future versions of python iirc
  source.write("if (wrapper_newup)")
  source.write("{")
  source.indent()
  source.write(f"PyErr_SetString(PyExc_TypeError, \"paxpython.{object_type} cannot be instantiated from python.\");")
  source.write(f"return NULL;")
  source.dedent()
  source.write("}")
  source.write(f"Py{object_type}Object *object = (Py{object_type}Object *)type->tp_alloc(type, 0);")
  source.write("return (PyObject*)object;")
else:
  source.write(f"Py{object_type}Object *object = (Py{object_type}Object *)type->tp_alloc(type, 0);")
  source.write("if (wrapper_newup)")
  source.indent()
  source.write(f"object->ref = new {object_type}();")
  source.dedent()
  source.write("object->tracked = false;")
  source.write("return (PyObject*)object;")
source.dedent()
source.write("}")
source.write()
# generate 'getter' functions for attributes 
#TODO: setters
source.write("// attribute functions")
for r in attrs:
  name = r["name"]
  attr_type = r["type"]

  # getter
  source.write(f"static PyObject* get_{name}(Py{object_type}Object *self, void *closure)")
  source.write("{")
  source.indent()
  if   attr_type == "uint64_t":
    source.write(f"return PyLong_FromUnsignedLongLong(self->ref->{name});")
  elif attr_type == "double":
    source.write(f"return PyFloat_FromDouble(self->ref->{name});")
  elif attr_type == "bool":
    source.write(f"return PyBool_FromLong(self->ref->{name});")
  elif attr_type in subtypes + [object_type]:
    if r["ptr"]:
      source.write(f"if (!self->ref->{name})")
      source.indent()
      source.write("Py_RETURN_NONE;")
      source.dedent()
      source.write(f"return (PyObject*)pyobjectize_{attr_type.lower()}(self->ref->{name});")
    else:
      source.write(f"return (PyObject*)pyobjectize_{attr_type.lower()}(&self->ref->{name});")
  elif attr_type == "fixeddistance":
    source.write(f"return PyFloat_FromDouble(DISTANCE_FIXED_TO_M(self->ref->{name}));")
  elif attr_type == "fixedtime":
    source.write(f"return PyFloat_FromDouble(TIME_FIXED_TO_S(self->ref->{name}));")
  elif attr_type == "celestialmass":
    source.write(f"return PyFloat_FromDouble(CELESTIALMASS_TO_KG(self->ref->{name}));")
  elif attr_type == "QString":
    source.write(f"return PyUnicode_FromString(self->ref->{name}.toStdString().c_str());")
  elif attr_type == "QList":
    # TODO: cache this list somehow instead of re-genning on every access? unclear how...
    template_type = r["template_type"]
    source.write(f"PyObject *{name}_pylist = PyList_New(0);")
    source.write(f"foreach ({template_type} *element, self->ref->{name})")
    source.write("{")
    source.indent()
    source.write(f"Py{template_type}Object *py_element = pyobjectize_{template_type.lower()}(element);")
    source.write(f"PyList_Append({name}_pylist, (PyObject *)py_element);")
    source.write(f"Py_DECREF(py_element);")
    source.dedent()
    source.write("}")
    source.write(f"return {name}_pylist;")
  else:
    raise TypeError(f"Unrecognized type: {attr_type}")
  #
  source.dedent()
  source.write("}")
  
  # setter
  # if read only, dont define the setter at all
  if r["readonly"]:
    continue
  source.write(f"static int set_{name}(Py{object_type}Object *self, PyObject *value, void *closure)")
  source.write("{")
  source.indent()
  source.write("if (value == NULL)")
  source.write("{")
  source.indent()
  source.write("PyErr_SetString(PyExc_TypeError, \"Cannot delete attribute.\");")
  source.write("return -1;")
  source.dedent()
  source.write("}")
  if   attr_type == "uint64_t":
    source.write(f"uint64_t v = PyLong_AsUnsignedLongLong(value);")
    source.write(f"PyObject *exception = PyErr_Occurred();")
    source.write("if (exception)")
    source.indent()
    source.write(f"return -1;")
    source.dedent()
    source.write(f"self->ref->{name} = v;")
  elif attr_type == "double":
    source.write(f"double v = PyFloat_AsDouble(value);")
    source.write(f"PyObject *exception = PyErr_Occurred();")
    source.write("if (exception)")
    source.indent()
    source.write(f"return -1;")
    source.dedent()
    source.write(f"self->ref->{name} = v;")
  elif attr_type == "bool":
    source.write("int v = PyObject_IsTrue(value);")
    source.write("if (v == -1)")
    source.indent()
    source.write("return -1;")
    source.dedent()
    source.write(f"self->ref->{name} = v;")
  elif attr_type in subtypes + [object_type]:
    source.write(f"if (!PyObject_IsInstance(value, (PyObject *)&Py{attr_type}Type))")
    source.write("{")
    source.indent()
    source.write(f"PyErr_SetString(PyExc_TypeError, \"Can only set value to {attr_type}.\");")
    source.write("return -1;")
    source.dedent()
    source.write("}")
    source.write(f"Py{attr_type}Object *v = (Py{attr_type}Object*)value;")
    if r["ptr"]:
      source.write(f"self->ref->{name} = v->ref;")
    else:
      source.write(f"self->ref->{name} = *v->ref;")
  elif attr_type == "fixeddistance":
    source.write(f"PyErr_SetString(PyExc_NotImplementedError, \"Setter for bool type not implemented.\");")
    source.write("return -1;")
  elif attr_type == "fixedtime":
    source.write(f"double v = PyFloat_AsDouble(value);")
    source.write(f"PyObject *exception = PyErr_Occurred();")
    source.write("if (exception)")
    source.indent()
    source.write(f"return -1;")
    source.dedent()
    source.write(f"self->ref->{name} = SECONDS_TO_TIME(v);")
  elif attr_type == "celestialmass":
    source.write(f"PyErr_SetString(PyExc_NotImplementedError, \"Setter for bool type not implemented.\");")
    source.write("return -1;")
  elif attr_type == "QString":
    if r["ptr"]:
      source.write(f"PyErr_SetString(PyExc_NotImplementedError, \"Setter for pointer to QString type not implemented.\");")
      source.write("return -1;")
    else:
      source.write("const char *c_str = PyUnicode_AsUTF8(value);")
      source.write("if (!c_str)")
      source.indent()
      source.write("return -1;")
      source.dedent()
      source.write("QString v = QString(c_str);")
      source.write(f"self->ref->{name} = v;")
  elif attr_type == "QList": #TODO: this may never be settable, in fact
    source.write(f"PyErr_SetString(PyExc_NotImplementedError, \"Setter for bool type not implemented.\");")
    source.write("return -1;")
  else:
    raise TypeError(f"Unrecognized type: {attr_type}")
  #
  source.write("return 0;")
  source.dedent()
  source.write("}")
# build getset array
if attrs:
  source.write("static PyGetSetDef getsets[] = {")
  source.indent()
  for r in attrs:
    source.write("{")
    name = r["name"]
    source.write(f"\"{name}\",")
    source.write(f"(getter)get_{name},")
    if r["readonly"]:
      source.write("NULL, // readonly")
    else:
      source.write(f"(setter)set_{name},")
    source.write("NULL, // documentation string")
    source.write("NULL, // closure")
    source.write("},")
  source.write("{NULL},")
  source.dedent()
  source.write("};")
source.write()
# build wrapped function calls
source.write("// wrapped function calls")
for f in funcs:
  name = f["name"]
  source.write(f"static PyObject *func_{name}(Py{object_type}Object *self, PyObject *args)")
  source.write("{")
  source.indent()
  argparse_string = ""
  for a in f["args"]:
    if   a["type"] == "double":
      attr_name = a["name"]
      source.write(f"double {attr_name};")
      argparse_string += "d"
  # build arg parser string
  if f["args"]:
    argptr_str = ",".join(["&" + a["name"] for a in f["args"]])
    source.write(f"if (!PyArg_ParseTuple(args, \"{argparse_string}\", {argptr_str}))")
    source.indent()
    source.write("return NULL;")
    source.dedent()
  args = ",".join([a["name"] for a in f["args"]])
  if   f["type"] == "double":
    source.write(f"return PyFloat_FromDouble(self->ref->{name}({args}));")
  elif f["type"] == "fixedtime":
    source.write(f"return PyFloat_FromDouble(((double)self->ref->{name}({args}))/TIME_FACTOR);")
  elif f["type"] == "fixedenergy": # TODO: i think for now this is just joules, not totally sure if thats really satisfactory
    source.write(f"return PyFloat_FromDouble(((double)self->ref->{name}({args})));")
  #TODO: return value
  #TODO: handle return type here
  source.dedent()
  source.write("}")
#build methoddef array
if funcs:
  source.write("static PyMethodDef  methods[] = {")
  source.indent()
  for f in funcs:
    name = f["name"]
    method = "METH_VARARGS" if f["args"] else "METH_NOARGS"
    source.write(f"{{\"{name}\", (PyCFunction)func_{name}, {method}, PyDoc_STR(\"Wraps a call to {name}.\")}},")
  source.write("{NULL, NULL}")
  source.dedent()
  source.write("};")
source.write()
# equality function which just checks the base wrapped pointer
source.write(f"static PyObject* __eq__(Py{object_type}Object *self, PyObject *other, int op)")
source.write("{")
source.indent()
source.write(f"if (!PyObject_IsInstance(other, (PyObject *)&Py{object_type}Type))")
source.indent()
source.write("Py_RETURN_FALSE;")
source.dedent()
source.write(f"Py{object_type}Object *other_cast = (Py{object_type}Object *)other;")
source.write(f"if (op == Py_EQ && self->ref == other_cast->ref)")
source.indent()
source.write("Py_RETURN_TRUE;")
source.dedent()
source.write("Py_RETURN_FALSE;")
source.dedent()
source.write("}")
source.write()
source.write(f"PyTypeObject Py{object_type}Type = {{")
source.indent()
source.write("PyVarObject_HEAD_INIT(NULL, 0)")
source.write(f".tp_name = \"paxpython.{object_type}\",")
source.write(f".tp_basicsize = sizeof(Py{object_type}Object),")
source.write(".tp_itemsize = 0,")
source.write(".tp_dealloc = (destructor)type_dealloc,")
# .tp_repr (__repr__) function would live here
source.write(".tp_flags = Py_TPFLAGS_DEFAULT,")
source.write(f".tp_doc = PyDoc_STR(\"PaxPython {object_type} Type Wrapper.\"),")
source.write(".tp_richcompare = (richcmpfunc)&__eq__,")
if funcs:
  source.write(".tp_methods = methods,")
if attrs:
  source.write(".tp_getset = getsets,")
source.write(".tp_new = type_new,")
source.dedent()
source.write("};")
source.write()
source.write("// Takes pointer to object, returns python wrapper for object with ref count of 1")
source.write(f"Py{object_type}Object *pyobjectize_{object_type.lower()}({object_type} *obj)")
source.write("{")
source.indent()
source.write("wrapper_newup = false;")
source.write(f"Py{object_type}Object *pyobj = (Py{object_type}Object *)PyObject_Call((PyObject *)&Py{object_type}Type,PyTuple_New(0),NULL);")
source.write("pyobj->ref = obj;")
source.write("pyobj->tracked = true;") # disables garbage collection of tracked object
source.write("wrapper_newup = true;")
source.write("return pyobj;")
source.dedent()
source.write("}")
source.write()
source.write("// Inits the type and adds it as a member to module")
source.write("// On failure returns false with exception set and decrefs module")
source.write(f"bool init_{object_type.lower()}(PyObject *m)")
source.write("{")
source.indent()
source.write(f"if (PyType_Ready(&Py{object_type}Type) < 0)")
source.write("{")
source.indent()
source.write("Py_DECREF(m);")
source.write("return false;")
source.dedent()
source.write("}")
source.write(f"Py_INCREF(&Py{object_type}Type);")
source.write(f"if (PyModule_AddObject(m, \"{object_type}\", (PyObject *)&Py{object_type}Type) < 0)")
source.write("{")
source.indent()
source.write(f"Py_DECREF(&Py{object_type}Type);")
source.write("Py_DECREF(m);")
source.write("return false;")
source.dedent()
source.write("}")
source.write("return true;")
source.dedent()
source.write("}")
source.close()
