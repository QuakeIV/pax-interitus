import json5, os

class loaded_cfg:
  def __init__(self):
    # set up some but not all default values
    self.base_type = None

#

def unknown_attr_check(filename, thing):
  if thing:
    raise KeyError(f"unrecognized keys for file ({filename}): {list(thing.keys())}")
#

def add_attr_or_func(l, thing):
  for item in l:
    if item["name"] == thing["name"]:
      raise KeyError(f"cannot have two attrs or functions with the same name: {item}, {thing}")
    #
  #
  l.append(thing)
#

_loaded_filenames = {}

def parse_file(json_cfg, filename):
  cfg = loaded_cfg()

  cfg.path = filename

  cfg.type = json_cfg.pop("type")
  cfg.type_header = json_cfg.pop("header")

  # load base type
  if "base_type" in json_cfg:
    # normalize path, so relative paths from the location of the current json will work
    # this path will be resolved later in resolve_references
    cfg.base_type = os.path.join(os.path.dirname(filename), json_cfg.pop("base_type"))
  else:
    cfg.base_type = None
  #

  # load includes
  cfg.includes = []
  if "include" in json_cfg:
    for i in json_cfg.pop("include"):
      # normalize path, so relative paths from the location of the current json will work
      # this path will be resolved later in resolve_references
      cfg.includes.append(os.path.abspath(os.path.join(os.path.dirname(filename), i)))
  #

  # instantiation config
  init_cfg = json_cfg.pop("init") if "init" in json_cfg else {"args":[]}
  # checking if its equal to false instead of logcal not-ing it, because other things might evaluate as logically false but not be what I am looking for, False is the specific desired value to look for here to see if instantiation is disabled
  cfg.instantiation_disabled = init_cfg == False
  if init_cfg:
    init_args_cfg = init_cfg.pop("args")
    unknown_attr_check(filename, init_cfg)
    cfg.init_args = []
    # args parameter is not optional
    if not isinstance(init_args_cfg, list):
      raise TypeError("init args must be list")
    for arg in init_args_cfg:
      a = {}
      a["name"] = arg.pop("name")
      t = arg.pop("type")
      t = t.split("*")
      a["type"] = t[0].strip()
      a["ptr"] = len(t) > 1 # if there is a * assume its a pointer
      unknown_attr_check(filename, arg)
      cfg.init_args.append(a)
  # not really worth unknown_attr_check on init_args_cfg, we already type check it is a list, and then iterate over it

  cfg.attrs = []
  if "attrs" in json_cfg:
    for r in json_cfg.pop("attrs"):
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
      unknown_attr_check(filename, r)
      add_attr_or_func(cfg.attrs, v)
    #
  #

  # TODO: resolve basetype functions
  cfg.funcs = []
  if "funcs" in json_cfg:
    for r in json_cfg.pop("funcs"):
      f = {}
      f["name"] = r.pop("name")
      f["type"] = r.pop("type")
      f["args"] = []
      for a in r.pop("args"):
        arg = {}
        arg["name"] = a.pop("name")
        t = a.pop("type")
        t = t.split("*")
        arg["ptr"] = len(t) > 1 # if there is a * assume its a pointer
        arg["type"] = t[0].strip()
        f["args"].append(arg)
      add_attr_or_func(cfg.funcs, f)
  #

  # error on unrecognized keys
  unknown_attr_check(filename, json_cfg)

  return cfg
#


def get_file(filename):
  filename = os.path.abspath(filename)

  # TODO: try-except this? more effecient but feels wrong (and not needed)
  if filename in _loaded_filenames:
    return _loaded_filenames[filename]

  with open(filename) as f:
    json_cfg = json5.load(f)
  #

  cfg = parse_file(json_cfg, filename)

  _loaded_filenames[filename] = cfg

  return cfg
#


# will also load any unloaded included files
def resolve_references(loaded_cfgs):
  not_traversed = list(loaded_cfgs)
  traversed = []

  while not_traversed:
    c = not_traversed.pop(0)
    if c.base_type:
      c.base_type = get_file(c.base_type)
      if c.base_type not in traversed + not_traversed:
        not_traversed.append(c.base_type)
    #

    resolved_includes = []
    for i in c.includes:
      inc_cfg = get_file(i)
      resolved_includes.append(inc_cfg)
      if inc_cfg not in traversed + not_traversed:
        not_traversed.append(inc_cfg)
    #

    c.includes = resolved_includes
    traversed.append(c)
  #
#


def find_known_types(cfg):
  traversed = []
  not_traversed = [cfg]

  while not_traversed:
    c = not_traversed.pop(0)
    if c.base_type and c.base_type not in traversed and c.base_type not in not_traversed:
      not_traversed.append(c.base_type)
    for i in c.includes:
      if i not in traversed:
        if i not in traversed and i not in not_traversed:
          not_traversed.append(i)
    traversed.append(c)
  #

  return [c.type for c in traversed]
#


def resolve_inheritance(cfg):
  stack = []
  c = cfg
  while c.base_type:
    stack.append(c)
    c = c.base_type
  stack.append(c) # last base type
  attrs = []
  funcs = []
  while stack:
    c = stack.pop()
    for a in c.attrs:
      if a["name"] in [i["name"] for i in attrs]:
        raise AttributeError(f"{c.type} ({c.path}) redefines attribute of base type ({c.base_type.path}): {a}")
      attrs.append(a)
    #
    for f in c.funcs:
      if f["name"] in [i["name"] for i in funcs]:
        raise AttributeError(f"{c.type} ({c.path}) redefines function of base type ({c.base_type.path}): {f}")
      funcs.append(f)
    #
  #
  cfg.attrs = attrs
  cfg.funcs = funcs
#


def validate(cfg):
  for a in cfg.attrs:
    if a["type"] not in ["QString", "QList", "bool", "double", "celestialmass", "fixeddistance", "fixedtime", "uint64_t"] + cfg.known_types:
      raise TypeError(f"Unrecognized type: {t[0].strip()} ({filename})")
    if a["type"] == "QList":
      if a["template_type"] not in [] + cfg.known_types:
        raise TypeError(f"Template type {v['template_type']} not handled for QList ({filename})")
      if not a["template_type_ptr"]:
        raise TypeError(f"Template type {v['template_type']} must be pointer for QList ({filename})")
  for f in cfg.funcs:
    if f["type"] not in ["double", "bool", "fixedtime", "fixedenergy", "QString","void"]:
      raise TypeError(f"Unrecognized return type: {f['type']} ({filename})")
    for a in f["args"]:
      if a["type"] not in ["double", "int"] + cfg.known_types:
        raise TypeError(f"Unrecognized arg type: {arg['type']} ({filename})")
      #
    #
  #
#


def load_files(cfg_filenames):
  loaded_cfgs = [get_file(os.path.abspath(filename)) for filename in cfg_filenames]

  resolve_references(loaded_cfgs)

  for cfg in loaded_cfgs:
    cfg.known_types = find_known_types(cfg)
  #

  for cfg in loaded_cfgs:
    resolve_inheritance(cfg)
  #

  for cfg in loaded_cfgs:
    validate(cfg)
  #

  return loaded_cfgs
#
