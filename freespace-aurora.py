#!/usr/bin/python3

# TODO: kindof temporary, if we pyinstaller this for instance this should go into the pyinstaller spec file
import sys, os
sys.path.append(os.path.abspath(os.path.dirname(__file__)))

from PySide6.QtCore import QSize, Qt
from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton, QToolBar, QVBoxLayout, QHBoxLayout, QWidget, QMenu
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtGui import QAction, QIcon, QCursor
import shiboken6

from build import libpaxpython
from build import ui # pyuic autogenned .py scripts

from techwindow import TechWindow
from celestialwindow import CelestialWindow
from spacecraftdesigner import SpacecraftDesigner
from capacitordesigner import CapacitorDesigner
from spacecraftwindow import SpacecraftWindow

app = QApplication()

# lazy solution to finding our icons
# TODO: really need a better way to let qt designer and also this script see our icons without piling everything into the root directory
os.chdir("ui")

# kill when ctrl-c is pressed (annoying when this doesnt work)
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL) # apparently thats sig_default and it causes the 'default action' to be taken by the OS

class MainWindow(QMainWindow):
  def __init__(self):
    super().__init__()
    self.ui = ui.mainwindow.Ui_MainWindow()
    # TODO: would be really nice to eventually have custom window icons for each kind of window, or at least to begin with broad categories
    self.ui.setupUi(self)

    #self.setWindowTitle("Pax Test")
    
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
    
    self.ui.actionTechWindow.triggered.connect(lambda: TechWindow(self).show())
    self.ui.actionSpacecraftDesigner.triggered.connect(lambda: SpacecraftDesigner(self).show())
    self.ui.techbutton.clicked.connect(lambda: TechWindow(self).show())
    self.ui.shipdesignbutton.clicked.connect(lambda: SpacecraftDesigner(self).show())
    self.ui.capacitordesignbutton.clicked.connect(lambda: CapacitorDesigner(self).show())
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
    if not planets and not ships:
      return
    m = QMenu(self)
    m.setAttribute(Qt.WA_DeleteOnClose)
    for p in planets:
      submenu = m.addMenu(p.name)
      # p=p needed to capture copy of planet instead of everyone sharing last value that was iterated over
      submenu.addAction("Focus", lambda p=p: self.renderer.set_focus(p))
      submenu.addAction("Info", lambda p=p: CelestialWindow(p,self).show())
    if planets and ships:
      m.addSeparator()
    for s in ships:
      submenu = m.addMenu(s.name)
      # s=s needed to capture copy of planet instead of everyone sharing last value that was iterated over
      submenu.addAction("Focus", lambda s=s: self.renderer.set_focus(s))
      submenu.addAction("Info", lambda s=s: SpacecraftWindow(s,self).show())
    m.popup(QCursor.pos())
  #
#

window = MainWindow()
window.show()

app.exec()
