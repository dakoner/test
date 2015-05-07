import copy
from PySide import QtGui, QtCore
import time
import sys
import numpy as np
import cv2



WIDTH = 1280
HEIGHT = 720
class MainWindow(QtGui.QMainWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)

        self.cam = cv2.VideoCapture(0)
        self.cam.set(3, WIDTH)
        self.cam.set(4, HEIGHT)
        ret, self.prev = self.cam.read()
        self.image = QtGui.QImage(self.prev.data, WIDTH, HEIGHT, QtGui.QImage.Format_RGB888)
        self.pix = QtGui.QPixmap.fromImage(self.image)

        self.lbl = QtGui.QLabel(self)
        self.lbl.setPixmap(self.pix)
        self.lbl.show()

        self.widget = QtGui.QWidget(self)
        self.hbox = QtGui.QHBoxLayout()
        self.widget.setLayout(self.hbox)    
        self.hbox.addStretch(1)
        self.hbox.addWidget(self.lbl)
        self.setCentralWidget(self.widget)
       

        self.timer = QtCore.QTimer()
        self.timer.start(.04)
        self.connect(self.timer, QtCore.SIGNAL('timeout()'), self.updateData) 

        self.fgbg = cv2.BackgroundSubtractorMOG(500, 6, 0.9, 1)
        self.fgbg = cv2.BackgroundSubtractorMOG2()
        
## TODO(dek): run at shutdown
#        cam.release()
#        cv2.destroyAllWindows()

    def updateData(self):
        ret, img = self.cam.read()
        p = cv2.cvtColor(self.prev, cv2.COLOR_BGR2GRAY)
        i = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        flow = cv2.calcOpticalFlowFarneback(p, i, 0.5, 3, 15, 3, 5, 1.2, 0)
        w, h = WIDTH, HEIGHT
        step = 16
        y, x = np.mgrid[step/2:h:step, step/2:w:step].reshape(2,-1)
        fx, fy = flow[y,x].T
        polys = np.vstack([x, y, x+fx, y+fy]).T.reshape(-1, 2, 2)
        polys = np.int32(polys + 0.5)

        self.prev = img


        # fgmask = self.fgbg.apply(img)
        # draw = img & fgmask
        self.image = QtGui.QImage(img.data, WIDTH, HEIGHT, QtGui.QImage.Format_RGB888)
        painter = QtGui.QPainter(self.image)
        blue = QtGui.QColor()
        blue.setNamedColor("blue")
        painter.setPen(QtGui.QPen(blue))
        for lines in polys:
            p1 = QtCore.QPoint(*lines[0])
            p2 = QtCore.QPoint(*lines[1])
            l = QtCore.QLine(p1, p2)
            painter.drawLine(l)

        self.pix.convertFromImage(self.image)
        self.repaint()
    
                        
if __name__ == '__main__':
    app = QtGui.QApplication([])
    window = MainWindow()
    window.resize(WIDTH, HEIGHT)
    window.show()

    app.exec_()

    
    
