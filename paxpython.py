#pax python wrapper (this will ideally incorporate the autocode crap we intend on as well)

import libpaxpython, shiboken6

# react to sigint (wires sigint to sig_dfl which it apparently already reacts to
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

# wraps the shiboken crap since that segfaults if you try to use it in C++ land
def bindToQApp(app):
    #TODO: type checking
    libpaxpython.bindToQApp(shiboken6.getCppPointer(app)[0])

def addSystemRendererToLayout(layout):
    libpaxpython.addSystemRendererToLayout(shiboken6.getCppPointer(layout)[0])

# mainwindow or widget or whatever
def defineRootWidget(widget):
    libpaxpython.defineRootWidget(shiboken6.getCppPointer(widget)[0])
