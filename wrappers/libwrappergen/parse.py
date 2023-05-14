import json5

class loaded_cfg:
  def __init__(self):
    pass
#

def unknown_attr_check(thing):
  if thing:
    raise KeyError(f"unrecognized keys for file ({args.i}): {list(thing.keys())}")
#

def load(filename):
  with open(filename) as f:
    json_cfg = json5.load(f)
  #
  cfg = loaded_cfg()

  cfg.object_type = json_cfg.pop("type")

  # this is used to declare structured subtypes that are also wrapped in python objects
  # this drives header includes and type validation
  cfg.subtypes = json_cfg.pop("subtypes") if "subtypes" in json_cfg else []

  # instantiation config
  init_cfg = json_cfg.pop("init") if "init" in json_cfg else {"args":[]}
  # checking if its equal to false instead of logcal not-ing it, because other things might evaluate as logically false but not be what I am looking for, False is the specific desired value to look for here to see if instantiation is disabled
  cfg.instantiation_disabled = init_cfg == False
  if init_cfg:
    init_args_cfg = init_cfg.pop("args")
    unknown_attr_check(init_cfg)
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
      unknown_attr_check(arg)
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
      if v["type"] not in ["QString", "QList", "bool", "double", "celestialmass", "fixeddistance", "fixedtime", "uint64_t"] + cfg.subtypes + [cfg.object_type]:
        raise TypeError(f"Unrecognized type: {t[0].strip()}")
      if v["type"] == "QList":
        if v["template_type"] not in [] + cfg.subtypes + [cfg.object_type]:
          raise TypeError(f"Template type {v['template_type']} not handled for QList")
        if not v["template_type_ptr"]:
          raise TypeError(f"Template type {v['template_type']} must be pointer for QList")
      unknown_attr_check(r)
      cfg.attrs.append(v)
    #
  #

  cfg.funcs = []
  if "funcs" in json_cfg:
    for r in json_cfg.pop("funcs"):
      f = {}
      f["name"] = r.pop("name")
      f["type"] = r.pop("type")
      if f["type"] not in ["double", "bool", "fixedtime", "fixedenergy", "QString","void"]:
        raise TypeError(f"Unrecognized return type: {f['type']}")
      f["args"] = []
      for a in r.pop("args"):
        arg = {}
        arg["name"] = a.pop("name")
        t = a.pop("type")
        t = t.split("*")
        arg["ptr"] = len(t) > 1 # if there is a * assume its a pointer
        arg["type"] = t[0].strip()
        if arg["type"] not in ["double", "int"] + cfg.subtypes + [cfg.object_type]:
          raise TypeError(f"Unrecognized arg type: {arg['type']}")
        f["args"].append(arg)
      cfg.funcs.append(f)
  #

  cfg.type_header = json_cfg.pop("header")

  # error on unrecognized keys
  unknown_attr_check(json_cfg)

  return cfg
#
