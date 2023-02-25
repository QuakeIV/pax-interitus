import os, importlib

__all__ = [os.path.splitext(m)[0] for m in os.listdir(os.path.dirname(__file__)) if (m.endswith(".py") and m != "__init__.py")]

for a in __all__:
  # have to use weird syntax to import relatively
  globals()[a] = importlib.import_module("."+a,__name__)
