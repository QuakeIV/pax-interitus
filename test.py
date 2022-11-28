#!/usr/bin/python3

from PySide6.QtCore import QSize, Qt
from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton, QToolBar, QVBoxLayout, QHBoxLayout, QWidget, QMenu
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtGui import QAction, QIcon, QCursor
import shiboken6
from build.python import libpaxpython

app = QApplication()

# kill when ctrl-c is pressed (annoying when this doesnt work)
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL) # apparently thats sig_default and it causes the 'default action' to be taken by the OS



class MainWindow(QMainWindow):
  def __init__(self):
    super().__init__()

    self.setWindowTitle("Pax Test")

    #opengl = QOpenGLWidget()
    
    #sizing
    #self.setFixedSize(QSize(400,300))
    self.resize(QSize(400,300))
    
    menu = self.menuBar()
    file_menu = menu.addMenu("File") #leading with & will underline the first letter
    test_action = QAction("Test action",self)
    test_action.setStatusTip("test tooltip") # no worky?
    test_action.triggered.connect(self.test_action)
    file_menu.addAction(test_action)
    
    layout = QHBoxLayout()
    self.renderer = libpaxpython.SystemRenderer(shiboken6.getCppPointer(layout)[0])
    self.renderer.right_click_callback = self.test_action

    widget = QWidget()
    widget.setLayout(layout)
    self.setCentralWidget(widget)
  #
  
  def test_action(self, poop):
    m = QMenu("derp", self)
    m.setAttribute(Qt.WA_DeleteOnClose)
    for p in poop:
      # p=p needed to capture copy of planet instead of everyone sharing last value that was iterated over
      m.addAction(p.name, lambda p=p: print(f"fart " + p.name))
    m.popup(QCursor.pos())
    print("poop")
  #
#

window = MainWindow()
window.show()

app.exec()
