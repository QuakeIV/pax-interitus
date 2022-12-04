#!/usr/bin/python3

from PySide6.QtCore import QSize, Qt
from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton, QToolBar, QVBoxLayout, QHBoxLayout, QWidget, QMenu
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtGui import QAction, QIcon, QCursor

from build.python import libpaxpython
from build.python import ui_spacecraftdesigner # pyuic autogenned .py script

# kill when ctrl-c is pressed (annoying when this doesnt work)
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL) # apparently thats sig_default and it causes the 'default action' to be taken by the OS

class SpacecraftDesigner(QMainWindow):
  def __init__(self, parent=None):
    super().__init__(parent)
    
    self.ui = ui_spacecraftdesigner.Ui_SpacecraftDesigner()
    self.ui.setupUi(self)
  #
#
