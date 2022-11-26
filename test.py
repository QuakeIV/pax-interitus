#!/usr/bin/python3

from PySide6.QtCore import QSize, Qt
from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton, QToolBar, QVBoxLayout, QWidget
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtGui import QAction, QIcon
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
    
    menu = self.menuBar()
    file_menu = menu.addMenu("File") #leading with & will underline the first letter
    test_action = QAction("Test action",self)
    test_action.setStatusTip("test tooltip") # no worky?
    test_action.triggered.connect(self.test_action)
    file_menu.addAction(test_action)
    
    layout = QVBoxLayout()
    self.renderer = libpaxpython.SystemRenderer(shiboken6.getCppPointer(layout)[0])
    self.renderer.right_click_callback = self.test_action

    widget = QWidget()
    widget.setLayout(layout)
    self.setCentralWidget(widget)
    
    #libpaxpython.test(shiboken6.getCppPointer(self)[0])
    
#    libpaxpython.addSystemRenderer(shiboken6.getCppPointer(opengl)[0])

    #self.setCentralWidget(opengl)
  #
  
  def test_action(self):
    print("poop")
  #
#

window = MainWindow()
window.show()

app.exec()
