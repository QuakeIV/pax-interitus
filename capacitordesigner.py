#!/usr/bin/python3

from PySide6.QtCore import QSize, Qt, QTimer
from PySide6.QtWidgets import QApplication, QDialog, QMainWindow, QPushButton, QToolBar, QVBoxLayout, QHBoxLayout, QWidget, QMenu
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtGui import QAction, QIcon, QCursor

from build import libpaxpython
from build import ui # pyuic autogenned .py scripts
import units

# kill when ctrl-c is pressed (annoying when this doesnt work)
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL) # apparently thats sig_default and it causes the 'default action' to be taken by the OS which kills the process

class CapacitorDesigner(QMainWindow):
  def __init__(self, parent=None):
    super().__init__(parent)
    
    self.ui = ui.capacitordesigner.Ui_CapacitorDesigner()
    self.ui.setupUi(self)
    self.setAttribute(Qt.WA_DeleteOnClose, True)
    
    self.ui.insulator.activated.connect(lambda: self.set_insulator(self.ui.insulator.currentData()))
    self.ui.capacity_edit.textEdited.connect(lambda: self.update())
    self.ui.voltage_edit.textEdited.connect(lambda: self.update())
    self.ui.amperage_edit.textEdited.connect(lambda: self.update())

    self.design = libpaxpython.CapacitorDesign()
    self.design.insulator = libpaxpython.universe.insulators[0]

    self.update()
  #
  
  def enterEvent(self, event):
    self.update()
  #
  
  def set_insulator(self, i):
    self.design.insulator = i
    self.update()
  #
  
  def update(self):
    new_insuls = libpaxpython.universe.insulators
    self.ui.insulator.clear()
    for i in new_insuls:
      if i.permittivity:
        self.ui.insulator.addItem(f"{i.name}", i)

    self.ui.insulator.setCurrentText(self.design.insulator.name)

    spec_voltage = units.parse_voltage(self.ui.voltage_edit.text())
    if spec_voltage == None:
      return
    #TODO: this needs to change if we ever introduce min separation
    separation = spec_voltage/self.design.insulator.strength;
    self.design.plate_separation = separation
    
    spec_capacity = units.parse_energy(self.ui.capacity_edit.text())
    if spec_capacity == None:
      return
    plate_area = (2 * spec_capacity * separation) / (spec_voltage * spec_voltage * self.design.insulator.permittivity);
    self.design.plate_area = plate_area
    
    spec_amperage = units.parse_amperage(self.ui.amperage_edit.text())
    if spec_amperage == None:
      return
    resistance = spec_voltage/spec_amperage;
    self.design.resistance = resistance
    
    #TODO: charge time
    
    self.ui.chargetime_label.setText(units.time_str(self.design.charge_time()))
    self.ui.volume_label.setText(units.volume_str(self.design.volume()))
    # TODO: always reports 0 because density is undefined for the materials, not useful
    #self.ui.tonnage_label.setText(tonnage_str(self.design.mass()))
    self.ui.capacitance_label.setText(units.capacitance_str(self.design.capacitance()))
    self.ui.area_label.setText(units.area_str(self.design.plate_area))
    self.ui.separation_label.setText(units.distance_str(self.design.plate_separation))
    self.ui.resistance_label.setText(units.resistance_str(self.design.resistance))
    self.ui.voltage_label.setText(units.voltage_str(self.design.max_voltage()))
    self.ui.capacity_label.setText(units.energy_str(self.design.energy_at_voltage(spec_voltage)))
    self.ui.amperage_label.setText(units.amperage_str(self.design.max_current(spec_voltage)))
  #
#
