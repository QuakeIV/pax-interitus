import math

# TODO: reevaluate, feels this is kindof lazy and retarded it just felt good to express it this way at the time
base_scale = {}
base_scale[1e-15] = "f"
base_scale[1e-12] = "p"
base_scale[1e-9]  = "n"
base_scale[1e-6]  = "μ"
base_scale[0.001] = "m"
base_scale[1]     = ""
base_scale[1000]  = "k"
base_scale[1e6]   = "M"
base_scale[1e9]   = "G"
base_scale[1e12]  = "T"
base_scale[1e15]  = "P"

def _find_scale_match(qty, scale_dict, target=1):
  scaled_qty = qty/target
  if not scaled_qty:
    scaled_qty = 1.0 # avoid div by zero
  # TODO: maybe just do this in C later if this profiles poorly compared to everything else
  # x + reciprocal of X means 1/5 of x is about as bad as 5x, coutneracting the problem of low values always winning in the sort
  scale = sorted(scale_dict, key = lambda x: abs(scaled_qty/x) + abs(x/scaled_qty))[0] #abs((target-(qty/x))/target)
  unit = scale_dict[scale] # this feels wasteful but not sure how to make better
  return scale, unit
#
def _parse_scale(s, scale_dict):
  s = s.strip()
  # TODO: actually handle or at least report parse fail for the suffix?
  for k, v in scale_dict.items():
    if s.endswith(v):
      try:
        return float(s[:-len(v)]) * k
      except:
        pass
#

def sigfig(val, fig=6):
  if not val:
    return 0.0
  dec = max(0,fig-int(math.log10(val)+1))
  return round(val, dec)

# mass-specific scales (note we are reporting kg from the game at base, so this is scaled weird
mass_scale = {k/1000:(v+"g") for k, v in base_scale.items()}
mass_scale[1e3]   = "kkg"
mass_scale[1e6]   = "mkg"
mass_scale[1e9]   = "bkg"
mass_scale[1e12]  = "tkg"
# astronomical units of mass
mass_scale[5.9722e24] = "M⊕" # mass of earth
mass_scale[1.9885e30] = "M☉" # mass of sun TODO: consider using ⨀, seems to look better since bigger, easier to tell from earth mass
def mass_str(qty):
  scale, unit = _find_scale_match(qty, mass_scale, 10.0)
  return f"{sigfig(qty/scale, 6)} {unit}"
#
# tonnage, similar but not identical to mass
# originally meant for like ships and maybe warhead strength
# this is currently used to express the amount of minerals on a planet and was tuned for such
tonnage_scale = {}
tonnage_scale[0.001] = "g"
tonnage_scale[1]     = "kg"
tonnage_scale[1e3]   = "t"
tonnage_scale[1e6]   = "kt"
tonnage_scale[1e9]   = "Mt"
tonnage_scale[1e12]  = "Gt"
tonnage_scale[1e15]  = "Tt"
tonnage_scale[1e18]  = "Pt"
tonnage_scale[1e21]  = "Et"
tonnage_scale[1e24]  = "Zt"
tonnage_scale[1e27]  = "Yt"
tonnage_scale[1e30]  = "Rt" # Ronna-Ton (lol) - circa 2022 adoption
tonnage_scale[1e33]  = "Qt" # quetta-ton (lolol) - circa 2022 adoption
# TODO: might eventually slip earth or solar masses in here? i think earth already falls somewhere in the middle of this scale though
def tonnage_str(qty):
  scale, unit = _find_scale_match(qty, tonnage_scale, 100.0)
  return f"{sigfig(qty/scale, 5)} {unit}"
#

dist_scale = {k:(v+"m") for k, v in base_scale.items()}
dist_scale[1e6]   = "kkm"
dist_scale[1e9]   = "mkm"
dist_scale[1e12]  = "bkm"
dist_scale[1e15]  = "tkm"
dist_scale[9.4607e15] = "Ly"
def distance_str(qty):
  scale, unit = _find_scale_match(qty, dist_scale, 250.0)
  return f"{sigfig(qty/scale, 6)} {unit}"
#
def parse_distance(s):
  return _parse_scale(s, dist_scale)
#

energy_scale = {k:(v+"J") for k, v in base_scale.items()}
def energy_str(qty):
  scale, unit = _find_scale_match(qty, energy_scale, 100.0)
  return f"{sigfig(qty/scale, 6)}{unit}"
#
def parse_energy(s):
  return _parse_scale(s, energy_scale)
#

capacitance_scale = {k:(v+"F") for k, v in base_scale.items()}
def capacitance_str(qty):
  scale, unit = _find_scale_match(qty, capacitance_scale, 10.0)
  return f"{sigfig(qty/scale, 6)}{unit}"
#

voltage_scale = {k:(v+"V") for k, v in base_scale.items()}
def voltage_str(qty):
  scale, unit = _find_scale_match(qty, voltage_scale, 10.0)
  return f"{sigfig(qty/scale, 6)}{unit}"
#
def parse_voltage(s):
  return _parse_scale(s, voltage_scale)
#

amperage_scale = {k:(v+"A") for k, v in base_scale.items()}
def amperage_str(qty):
  scale, unit = _find_scale_match(qty, amperage_scale, 10.0)
  return f"{sigfig(qty/scale, 6)}{unit}"
#
def parse_amperage(s):
  return _parse_scale(s, amperage_scale)
#

resistance_scale = {k:(v+"Ω") for k, v in base_scale.items()}
def resistance_str(qty):
  scale, unit = _find_scale_match(qty, resistance_scale, 10.0)
  return f"{sigfig(qty/scale, 6)}{unit}"
#

# TODO: general scaling improvements/additional units of area, this seems fine for now though
area_scale = {}
area_scale[0.000001] = "mm²"
area_scale[1] = "m²"
area_scale[1000000] = "km²"
def area_str(qty):
  scale, unit = _find_scale_match(qty, area_scale, 100.0)
  return f"{sigfig(qty/scale, 6)}{unit}"
#

# TODO: internal reference frame is in liters, this is translating to cubic meters as that is more familiar to a lot of space game nerds
# should we do everyting internally in m3? maybe not a lot of math appears to prefer liters? is that true at all?
# TODO: general scaling improvements, this seems fine for now though
volume_scale = {}
volume_scale[0.000001] = "mm³"
volume_scale[1000] = "m³"
volume_scale[1000000000000] = "km³"
def volume_str(qty):
  scale, unit = _find_scale_match(qty, volume_scale, 10.0)
  return f"{sigfig(qty/scale, 4)}{unit}"
#

#time scale (rather different in general)
# TODO: some way to have a decomposed form in terms of days, minutes, seconds, etc?
time_scale = {}
time_scale[1]    = "s"
time_scale[60]   = "mins"
time_scale[3600] = "hours"
time_scale[86400] = "days"
time_scale[31557600] = "years"
def time_str(qty):
  scale, unit = _find_scale_match(qty, time_scale, 50.0)
  return f"{sigfig(qty/scale, 4)} {unit}"
#
