
# TODO: reevaluate, feels this is kindof lazy and retarded it just felt good to express it this way at the time
base_scale = {}
base_scale[1e-15] = "f"
base_scale[1e-12] = "p"
base_scale[1e-9]  = "n"
base_scale[1e-6]  = "μ"
base_scale[0.001] = "m"
base_scale[1]     = ""
base_scale[1000]  = "k"
base_scale[1e6]   = "kk"
base_scale[1e9]   = "mk"
base_scale[1e12]  = "bk"
base_scale[1e15]  = "tk"

def _find_scale_match(qty, scale_dict, target=1):
  scaled_qty = qty/target
  # TODO: maybe just do this in C later if this profiles poorly compared to everything else
  # x + reciprocal of X means 1/5 of x is about as bad as 5x, coutneracting the problem of low values always winning in the sort
  scale = sorted(scale_dict, key = lambda x: abs(scaled_qty/x) + abs(x/scaled_qty))[0] #abs((target-(qty/x))/target)
  unit = scale_dict[scale] # this feels wasteful but not sure how to make better
  return scale, unit
#

# mass-specific scales
mass_scale = {k:(v+"g") for k, v in base_scale.items()}
# astronomical units of mass
mass_scale[5.9722e27] = "M⊕" # mass of earth
mass_scale[1.9885e33] = "M☉" # mass of sun TODO: consider using ⨀, seems to look better since bigger, easier to tell from earth mass
def mass_str(qty):
  scale, unit = _find_scale_match(qty, mass_scale, 10.0)
  return f"{qty/scale:.6g} {unit}"
#

dist_scale = {k:(v+"m") for k, v in base_scale.items()}
def distance_str(qty):
  scale, unit = _find_scale_match(qty, dist_scale, 1000.0)
  return f"{qty/scale:.6g} {unit}"
#
