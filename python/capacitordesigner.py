#!/usr/bin/python3

from PySide6.QtCore import QSize, Qt, QTimer
from PySide6.QtWidgets import QApplication, QDialog, QMainWindow, QPushButton, QToolBar, QVBoxLayout, QHBoxLayout, QWidget, QMenu
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtGui import QAction, QIcon, QCursor

from build.python import libpaxpython
from build.python import ui_capacitordesigner # pyuic autogenned .py script

# kill when ctrl-c is pressed (annoying when this doesnt work)
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL) # apparently thats sig_default and it causes the 'default action' to be taken by the OS which kills the process

class CapacitorDesigner(QDialog):
  def __init__(self, parent=None):
    super().__init__(parent)
    
    self.ui = ui_capacitordesigner.Ui_CapacitorDesigner()
    self.ui.setupUi(self)
    self.setAttribute(Qt.WA_DeleteOnClose, True)
    
    self.ui.insulator.activated.connect(lambda: self.set_insulator(self.ui.insulator.currentData()))
    
    self.selected_insulator = None

    self.update()
  #
  
  def enterEvent(self, event):
    self.update()
  #
  
  def set_insulator(self, i):
    self.selected_insulator = i
    self.update()
  #
  
  def update(self):
    # TODO: globally maintained lists that are called on periodic update loop by main app
    new_insuls = libpaxpython.test()
    self.ui.insulator.clear()
    for i in new_insuls:
      self.ui.insulator.addItem(f"{i.name}", i)
    if self.selected_insulator:
      self.ui.insulator.setCurrentText(self.selected_insulator.name)
    #
#
