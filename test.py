#!/usr/bin/python3
import sys, paxpython

from PySide6.QtWidgets import QApplication, QLabel, QPushButton, QWidget, QVBoxLayout

app = QApplication(sys.argv)

layout = QVBoxLayout()

button = QPushButton("Click me")
layout.addWidget(button)
paxpython.bindToQApp(app)
paxpython.addSystemRendererToLayout(layout)

widge = QWidget()
widge.setLayout(layout)
widge.show()



app.exec()
