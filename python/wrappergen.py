

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
    raise KeyError(f"unrecognized keys for file ({sys.argv[1]}): {list(thing.keys())}")
#
  


# json5 allows trailing commas
import sys, json5, os

# set default values, leave out non-optional values so there are errors downstream
cfg = {}

with open(sys.argv[1]) as f:
  cfg = json5.load(f)

object_type = cfg.pop("type")

attrs = []
for r in cfg.pop("attrs"):
  v = {}
  v["name"] = r.pop("name")
  t = r.pop("type")
  v["type"] = t
  if t not in ["QString", "double"]:
    raise TypeError(f"Unrecognized type: {t}")
  unknown_attr_check(r)
  attrs.append(v)
#

type_header = f"{object_type.lower()}.h"
if "header_override" in cfg:
  type_header = cfg.pop("header_override")
#

# error on unrecognized keys
unknown_attr_check(cfg)

header = FileWriter("py" + os.path.splitext(os.path.basename(sys.argv[1]))[0] + "wrapper.h")
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
header.dedent()
header.write(f"}} Py{object_type}Object;")
header.write()
header.write(f"extern PyTypeObject Py{object_type}Type;")
header.write()
header.write(f"#endif // PYWRAPPER_{object_type.upper()}_H")
header.close()

source = FileWriter("py" + os.path.splitext(os.path.basename(sys.argv[1]))[0] + "wrapper.cpp")

source.write("#include <Python.h>")
source.write("#include <structmember.h> // additional python context (forgot what exactly)")
source.write(f"#include \"py{object_type.lower()}wrapper.h\"")
source.write(f"#include \"{type_header}\"")
source.write()
source.write(f"static void type_dealloc(Py{object_type}Object *self)")
source.write("{")
source.indent()
source.write("Py_TYPE(self)->tp_free((PyObject *) self);")
source.dedent()
source.write("}")
source.write("static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)")
source.write("{")
source.indent()
source.write("return type->tp_alloc(type, 0);")
source.dedent()
source.write("}")
for r in attrs:
  name = r["name"]
  attr_type = r["type"]
  source.write(f"static PyObject* get_{name}(Py{object_type}Object *self, void *closure)")
  source.write("{")
  source.indent()
  if   attr_type == "QString":
    source.write(f"return PyUnicode_FromString(self->ref->{name}.toStdString().c_str());")
  elif attr_type == "double":
    source.write(f"return PyFloat_FromDouble(self->ref->{name});")
  else:
    raise TypeError(f"Unrecognized type: {attr_type}")
  #
  source.dedent()
  source.write("}")
#
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
source.write("static PyGetSetDef getsets[] = {")
source.indent()
for r in attrs:
  source.write("{")
  name = r["name"]
  source.write(f"\"{name}\",")
  source.write(f"(getter)get_{name},")
  source.write("NULL, // setter")
  source.write("NULL, // documentation string")
  source.write("NULL, // closure")
  source.write("},")
source.dedent()
source.write("};")
source.write(f"PyTypeObject Py{object_type}Type = {{")
source.indent()
source.write("PyVarObject_HEAD_INIT(NULL, 0)")
source.write(f".tp_name = \"paxpython.{object_type}\",")
source.write(f".tp_basicsize = sizeof(Py{object_type}Object),")
source.write(".tp_itemsize = 0,")
source.write(".tp_dealloc = (destructor)type_dealloc,")
# .tp_repr __repr__ function would live here
source.write(".tp_flags = Py_TPFLAGS_DEFAULT,")
source.write(f".tp_doc = PyDoc_STR(\"PaxPython {object_type} Type Wrapper\"),")
source.write(".tp_richcompare = (richcmpfunc)&__eq__,")
source.write(".tp_getset = getsets,")
source.write(".tp_new = type_new,")
source.dedent()
source.write("};")
source.close()
