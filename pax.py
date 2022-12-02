#!/usr/bin/python3

# TODO: kindof temporary, if we pyinstaller this for instance this should go into the pyinstaller spec file
import sys, os
sys.path.append(os.path.abspath(os.path.dirname(__file__)))

from PySide6.QtCore import QSize, Qt
from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton, QToolBar, QVBoxLayout, QHBoxLayout, QWidget, QMenu
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtGui import QAction, QIcon, QCursor
import shiboken6

from build.python import libpaxpython
from build.python import ui_mainwindow # pyuic autogenned .py script

app = QApplication()

# kill when ctrl-c is pressed (annoying when this doesnt work)
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL) # apparently thats sig_default and it causes the 'default action' to be taken by the OS

class MainWindow(QMainWindow):
  def __init__(self):
    super().__init__()
    
    self.ui = ui_mainwindow.Ui_MainWindow()
    self.ui.setupUi(self)

    self.setWindowTitle("Pax Test")
    
    #sizing
    #self.setFixedSize(QSize(400,300))
    #self.resize(QSize(400,300))
    
    self.ui.pausebutton.clicked.connect(self.pause)
    self.ui.pluswarp.clicked.connect(self.increment_warp)
    self.ui.minuswarp.clicked.connect(self.decrement_warp)
    self.ui.warp1.clicked.connect(self.warp_1)
    self.ui.warp1k.clicked.connect(self.warp_1k)
    self.ui.warp1M.clicked.connect(self.warp_1M)
    self.ui.warpmax.clicked.connect(self.warp_max)
    
    self.renderer = libpaxpython.SystemRenderer(shiboken6.getCppPointer(self.ui.horizontalLayout)[0])
    self.renderer.right_click_callback = self.rightclick
    
    #TODO: re-set this up with time data or some such?
    self.ui.statusbar.showMessage("Imma status bar (TODO)")
  #

  def update_warp_display(self):
    time_warp = libpaxpython.universe.time_warp
    if   time_warp <= -20:
      self.ui.displaywarp.setText(f"1/{1<<(-1*time_warp - 20)}Mx")
    elif time_warp <= -10:
      self.ui.displaywarp.setText(f"1/{1<<(-1*time_warp - 10)}kx")
    elif time_warp < 0:
      self.ui.displaywarp.setText(f"1/{1<<(-1*time_warp)}x")
    elif time_warp < 10:
      self.ui.displaywarp.setText(f"{1<<(time_warp)}x")
    elif time_warp < 20:
      self.ui.displaywarp.setText(f"{1<<(time_warp-10)}kx")
    elif time_warp < 30:
      self.ui.displaywarp.setText(f"{1<<(time_warp-20)}Mx")
  #
  
  def pause(self, checked):
    if checked:
      libpaxpython.universe.paused = True
      self.ui.pausebutton.setText("PAUSED")
    else:
      libpaxpython.universe.paused = False
      self.ui.pausebutton.setText("PAUSE")
  #
  
  def increment_warp(self):
    libpaxpython.universe.time_warp += 1
    self.update_warp_display()
  #
   
  def decrement_warp(self):
    libpaxpython.universe.time_warp -= 1
    self.update_warp_display()
  #
  
  def warp_1(self):
    libpaxpython.universe.time_warp = 0
    self.update_warp_display()
  #
  
  def warp_1k(self):
    libpaxpython.universe.time_warp = 10
    self.update_warp_display()
  #
  
  def warp_1M(self):
    libpaxpython.universe.time_warp = 20
    self.update_warp_display()
  #
  
  def warp_max(self):
    libpaxpython.universe.time_warp = libpaxpython.universe.max_warp
    self.update_warp_display()
  #
  
  def rightclick(self, planets, ships):
    m = QMenu(self)
    m.setAttribute(Qt.WA_DeleteOnClose)
    for p in planets:
      # p=p needed to capture copy of planet instead of everyone sharing last value that was iterated over
      m.addAction(p.name, lambda p=p: self.renderer.set_focus(p))
      print(p.name)
      print(p.radius)
      if p.parent:
        print("parent: " + p.parent.name)
      print(p.orbital_radius)
      print(p.children)
      print()
      
    for s in ships:
      # s=s needed to capture copy of planet instead of everyone sharing last value that was iterated over
      m.addAction(s.name, lambda s=s: self.renderer.set_focus(s))
    m.popup(QCursor.pos())
  #
#

window = MainWindow()
window.show()

app.exec()
