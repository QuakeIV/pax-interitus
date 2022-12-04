#!/usr/bin/python3

from PySide6.QtCore import QSize, Qt
from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton, QToolBar, QVBoxLayout, QHBoxLayout, QWidget, QMenu
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtGui import QAction, QIcon, QCursor

from build.python import libpaxpython
from build.python import ui_celestialwindow # pyuic autogenned .py script

# kill when ctrl-c is pressed (annoying when this doesnt work)
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL) # apparently thats sig_default and it causes the 'default action' to be taken by the OS

class CelestialWindow(QMainWindow):
  def __init__(self, celestial, parent=None):    
    if not isinstance(celestial, libpaxpython.Celestial):
      raise TypeError("must initialize celestialwindow with celestial type")
    super().__init__(parent)
    
    self.cel = celestial
    
    self.ui = ui_celestialwindow.Ui_CelestialWindow()
    self.ui.setupUi(self)
    
    
  #
#
