#!/usr/bin/python3

from PySide6.QtCore import QSize, Qt, QTimer
from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton, QToolBar, QVBoxLayout, QHBoxLayout, QWidget, QMenu
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtGui import QAction, QIcon, QCursor

from build import libpaxpython
from build import ui # pyuic autogenned .py scripts

import units

# kill when ctrl-c is pressed (annoying when this doesnt work)
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL) # apparently thats sig_default and it causes the 'default action' to be taken by the OS

class CelestialWindow(QMainWindow):
  def __init__(self, celestial, parent=None):
    if not isinstance(celestial, libpaxpython.Celestial):
      raise TypeError("must initialize celestialwindow with celestial type")
    super().__init__(parent)
    self.ui = ui.celestialwindow.Ui_CelestialWindow()
    self.ui.setupUi(self)
    self.setAttribute(Qt.WA_DeleteOnClose, True) #NOTE: vital for self-updating windows or zombie timers will be left behind

    self.cel = celestial

    self.update()

    self.timer = QTimer(self)
    self.timer.timeout.connect(self.update)
    self.timer.start(200)
  #

  def update(self):
    self.setWindowTitle(self.cel.name + " Info")
    self.ui.name.setText(self.cel.name)
    self.ui.mass.setText(units.mass_str(self.cel.mass))
    self.ui.physical_radius.setText(units.distance_str(self.cel.radius))
    # TODO: for now stars just dont have a trajectory, this might need to be fixed if that is no longer the long term strategy
    if self.cel.trajectory:
      self.ui.orbital_period.setText(units.time_str(self.cel.trajectory.orbital_period))
      self.ui.orbital_radius.setText(units.distance_str(self.cel.trajectory.orbital_radius))
    else:
      # TODO: arguably this could relate to some kind of galactic core relative trajectory, but isnt currently
      self.ui.orbital_period.setText("None")
      self.ui.orbital_radius.setText("None")
    # TODO: units.py implementation?
    self.ui.surface_gravity.setText(f"{self.cel.surface_gravity:.2f} m/s²")

    # minerals
    # TODO: for now stars just dont have minerals, this might need to be fixed if that is no longer the long term strategy
    if self.cel.mineralogy:
      self.ui.mineral_conventional.setText(units.tonnage_str(self.cel.mineralogy.quantity_conventional))
      self.ui.mineral_fuel.setText(units.tonnage_str(self.cel.mineralogy.quantity_fuel))
      self.ui.mineral_duranium.setText(units.tonnage_str(self.cel.mineralogy.quantity_duranium))
    else:
      self.ui.mineral_conventional.setText("None")
      self.ui.mineral_fuel.setText("None")
      self.ui.mineral_duranium.setText("None")
  #
#
