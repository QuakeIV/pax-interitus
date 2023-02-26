#!/usr/bin/python3

from PySide6.QtCore import QSize, Qt
from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton, QToolBar, QVBoxLayout, QHBoxLayout, QWidget, QMenu, QInputDialog
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtGui import QAction, QIcon, QCursor

from build import libpaxpython
from build import ui # pyuic autogenned .py scripts

# kill when ctrl-c is pressed (annoying when this doesnt work)
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL) # apparently thats sig_default and it causes the 'default action' to be taken by the OS

def get_spacecraft_design_name(d):
  if not d:
    return "No Design"
  ree = ""
  if d.type:
    ree += f"GT{d.type} " #TODO: un-hardcode the GT thing, just stealing from freespace 2 GTA for now, this should come from the current empire later
  #
  if not d.class_name:
    ree += "New Class"
  else:
    ree += f"{d.class_name} Class"
  return ree
#

def design_description(d):
  if not d:
    return "No design selected."
  #
  ree = ""
  ree += get_spacecraft_design_name(d)
  return ree
#

class SpacecraftDesigner(QMainWindow):
  def __init__(self, parent=None):
    super().__init__(parent)
    self.ui = ui.spacecraftdesigner.Ui_SpacecraftDesigner()
    self.ui.setupUi(self)
    self.setAttribute(Qt.WA_DeleteOnClose, True) #NOTE: vital for self-updating windows or zombie timers will be left behind

    self.ui.selectclass.activated.connect(self.selectclass_activated)
    self.ui.selecteddesign.activated.connect(self.selecteddesign_activated)

    self.ui.newdesign.clicked.connect(self.new_design)
    self.ui.deletedesign.clicked.connect(self.delete_design)
    self.ui.designname.clicked.connect(self.design_name)
    self.ui.markobsolete.clicked.connect(self.mark_obsolete)
    self.ui.showobsolete.clicked.connect(self.show_obsolete)

    self.design = None

    self.update()
  #
  
  def enterEvent(self, event):
    self.update()
  #
  
  def selectclass_activated(self, index):
    if self.design:
      self.design.type = self.ui.selectclass.currentText()
    self.update()
  #
  
  def selecteddesign_activated(self, index):
    current_data = self.ui.selecteddesign.currentData()
    if current_data:
      self.design = current_data
    self.update()
  #
  
  def new_design(self):
    d = libpaxpython.SpacecraftDesign()
    d.type = self.ui.selectclass.currentText()
    libpaxpython.universe.spacecraft_designs.append(d)
    self.design = d
    self.update()
  #
  
  def delete_design(self):
    if self.design:
      libpaxpython.universe.spacecraft_designs.remove(self.design)
      # TODO: would be nice if this picked above or below where the previous design was located in the list instead
      self.design = libpaxpython.universe.spacecraft_designs[0]
    self.update()
  #
  
  
  
  def design_name(self):
    text, ok = QInputDialog.getText(self, 'Design Name', 'Enter name:')
    if ok and self.design:
      self.design.class_name = text
      self.update()
    #
  #
  
  def mark_obsolete(self, checked = False):
    if self.design:
      if checked:
        self.design.obsolete = True
      else:
        self.design.obsolete = False
      #
    #
    self.update()
  #
  
  def show_obsolete(self, checked = False):
    if self.design:
      if checked:
        print("show obsolete designs")
      else:
        print("hide obsolte designs")
      #
    #
    self.update()
  #
  
  def update(self):
    if self.design not in libpaxpython.universe.spacecraft_designs:
      if libpaxpython.universe.spacecraft_designs:
        self.design = libpaxpython.universe.spacecraft_designs[0]
      else:
        # TODO: note this is currently necessary to avoid segfault, API side should except here probably
        self.design = None
      #
    #
    
    self.ui.designtext.setText(design_description(self.design))
    
    if self.design:
      self.setWindowTitle(f"Spacecraft Designer ({get_spacecraft_design_name(self.design)})")
    else:
      self.setWindowTitle("Spacecraft Designer")
    
    self.ui.selecteddesign.clear()
    if libpaxpython.universe.spacecraft_designs:
      idx = 0
      i = 0
      for d in libpaxpython.universe.spacecraft_designs:
        self.ui.selecteddesign.addItem(get_spacecraft_design_name(d), d)
        if d == self.design:
          idx = i
        i += 1
      #
      self.ui.markobsolete.setChecked(self.design.obsolete)
      self.ui.selecteddesign.setCurrentIndex(idx)
    else:
      self.ui.selecteddesign.addItem("None", None)
    #
    
    # re-set stuff that cannot be selected for 'no design'
    if not self.design:
      self.ui.markobsolete.setChecked(False)
    #
  #
#
