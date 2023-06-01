#!/usr/bin/python3

from PySide6.QtCore import QSize, Qt, QTimer
from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton, QToolBar, QVBoxLayout, QHBoxLayout, QWidget, QMenu
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtGui import QAction, QIcon, QCursor

from build import libpaxpython
from build import ui # pyuic autogenned .py scripts

# kill when ctrl-c is pressed (annoying when this doesnt work)
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL) # apparently thats sig_default and it causes the 'default action' to be taken by the OS

class SpacecraftWindow(QMainWindow):
  def __init__(self, spacecraft, parent=None):
    if not isinstance(spacecraft, libpaxpython.Spacecraft):
      raise TypeError("must initialize spacecraftwindow with spacecraft type")
    super().__init__(parent)
    self.ui = ui.spacecraftwindow.Ui_MainWindow()
    self.ui.setupUi(self)
    self.setAttribute(Qt.WA_DeleteOnClose, True)

    self.s = spacecraft
    self.jump_target = None
    
    self.timer = QTimer(self)
    self.timer.timeout.connect(self.update)
    #self.timer.start(200)
    
    self.update()
    
    self.ui.jumpbutton.clicked.connect(self.jump)
    self.ui.jumptarget.activated.connect(lambda: self.select_jump_target(self.ui.jumptarget.currentData()))
  #
  
  def select_jump_target(self, target):
    self.jump_target = target
    self.update()
  #
  
  def update(self):
    self.setWindowTitle("Spacecraft (" + self.s.name + ")")
    self.ui.name.setText(self.s.name)

    self.ui.jumptarget.clear()
    root = self.s.trajectory.solarsystem.root
    self.ui.jumptarget.addItem("None", None)
    self.ui.jumptarget.addItem(f"{root.name}", root)
    for c in root.children:
      self.ui.jumptarget.addItem(f"{c.name}", c)
    # just assuming if 'None' is selected, we are good to go and it will just stay on the first item, which is already 'None"
    if self.jump_target:
      self.ui.jumptarget.setCurrentText(self.jump_target.name) #TODO: bit unfortunate tbh (if naming changes)
      self.ui.jumpbutton.setEnabled(True)
    else:
      self.ui.jumpbutton.setEnabled(False)
    #TODO: close if referenced spacecraft is deleted from spacecraft list
    #self.close()
    pass
  #
  
  def jump(self):
    if self.jump_target:
      orbit = libpaxpython.OrbitType(self.jump_target, 200000 + self.jump_target.radius)
      self.s.trajectory = orbit
      print("jump jump jump")
      self.update()
  #
#
