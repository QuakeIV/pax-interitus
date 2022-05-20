#!/usr/bin/python3

from PySide6.QtWidgets import QApplication, QWidget
import paxpython
import sys

app = QApplication(sys.argv)

window = QWidget()
window.show()

print(dir(paxpython))
print(dir(paxpython.SystemRenderer))
print(paxpython.SystemRenderer)
print(issubclass(paxpython.SystemRenderer, QWidget))
rend = paxpython.SystemRenderer(window)

app.exec()
