#!/usr/bin/python3
import shiboken6, sys, paxpython

from PySide6.QtWidgets import QApplication, QLabel, QPushButton, QWidget, QVBoxLayout

app = QApplication(sys.argv)

layout = QVBoxLayout()

button = QPushButton("Click me")
layout.addWidget(button)
paxpython.bindToQApp(shiboken6.getCppPointer(app)[0])
paxpython.addSystemRendererToLayout(shiboken6.getCppPointer(layout)[0])

widge = QWidget()
widge.setLayout(layout)
widge.show()

print(dir(paxpython))

app.exec()
