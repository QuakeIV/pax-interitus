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
from build.python import ui_mainwindow # pyuic autogenned .py scripts

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
    
    #menu = self.menuBar()
    #file_menu = menu.addMenu("File") #leading with & will underline the first letter
    #test_action = QAction("Test action",self)
    #test_action.setStatusTip("test tooltip") # no worky?
    #test_action.triggered.connect(self.test_action)
    #file_menu.addAction(test_action)
    
    self.ui.pausebutton.clicked.connect(self.pause)
    self.ui.pluswarp.clicked.connect(self.increment_warp)
    self.ui.minuswarp.clicked.connect(self.decrement_warp)
    self.ui.warp1.clicked.connect(self.warp_1)
    self.ui.warp1k.clicked.connect(self.warp_1k)
    self.ui.warp1M.clicked.connect(self.warp_1M)
    self.ui.warpmax.clicked.connect(self.warp_max)
    
    #layout = QHBoxLayout()
    #warpwidget = TimeWarpWidget()
    #layout.addWidget(warpwidget)
    #self.renderer = libpaxpython.SystemRenderer(shiboken6.getCppPointer(layout)[0])
    self.renderer = libpaxpython.SystemRenderer(shiboken6.getCppPointer(self.ui.horizontalLayout)[0])
    self.renderer.right_click_callback = self.test_action
    
    #TODO: update warp display

    widget = QWidget()
#    widget.setLayout(layout)
#    self.setCentralWidget(widget)
  #
  
  def pause(self, checked):
    if checked:
      print("TODO: implement pausing")
    else:
      print("TODO: implement unpausing")
  #
  
  def increment_warp(self):
    print("TODO: implement increasing time warp")
  #
   
  def decrement_warp(self):
    print("TODO: implement decreasing time warp")
  #
  
  def warp_1(self):
    print("TODO: implement warp 1")
    #TODO: update warp display
  #
  
  def warp_1k(self):
    print("TODO: implement warp 1k")
  #
  
  def warp_1M(self):
    print("TODO: implement warp 1M")
  #
  
  def warp_max(self):
    print("TODO: implement max warp")
  #
  
  def test_action(self, planets, ships):
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
