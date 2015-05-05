from PySide import QtGui, QtCore
import time
import sys
sys.path.insert(0, "/home/dek/ImageJ")
import numpy as np
import cv2
import MMCorePy

class MM(QtCore.QThread):
    def __init__(self):
        super(MM, self).__init__()
        self.mmc = MMCorePy.CMMCore()
        self.mmc.enableStderrLog(True)
        self.mmc.enableDebugLog(True)

        self.mmc.loadSystemConfiguration("test.cfg")
        self.mmc.setProperty("ArduinoNeoPixel-Shutter", 'Intensity', '100')
        self.mmc.startContinuousSequenceAcquisition(1)
        self.img = self.mmc.getLastImage()

    def run(self):
        while True:
            if self.mmc.getRemainingImageCount() > 0:
                self.img = self.mmc.getLastImage()
            else:
                print('No frame')

            time.sleep(0.01)

    def getImage(self):
        return self.img
    
    def stop(self):
        self.mmc.stopSequenceAcquisition()
        self.mmc.reset()

        
class MainWindow(QtGui.QMainWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)


        self.mm = MM()
        self.prev = self.mm.getImage()
        self.mm.start()

        prevrgb = cv2.cvtColor(self.prev, cv2.COLOR_GRAY2RGB)
        self.image = QtGui.QImage(prevrgb.data, 640, 480, QtGui.QImage.Format_RGB888)
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

        # self.fgbg = cv2.BackgroundSubtractorMOG(500, 6, 0.9, 1)
        self.fgbg = cv2.BackgroundSubtractorMOG2()
        

    def updateData(self):
        img = self.mm.getImage()

        flow = cv2.calcOpticalFlowFarneback(self.prev, img, 0.5, 3, 15, 3, 5, 1.2, 0)
        w, h = 640, 480
        step = 16
        y, x = np.mgrid[step/2:h:step, step/2:w:step].reshape(2,-1)
        fx, fy = flow[y,x].T
        polys = np.vstack([x, y, x+fx, y+fy]).T.reshape(-1, 2, 2)
        polys = np.int32(polys + 0.5)

        self.prev = img


        # fgmask = self.fgbg.apply(img)
        # draw = img & fgmask
        image = cv2.cvtColor(img, cv2.COLOR_GRAY2RGB)
        self.image = QtGui.QImage(image.data, 640, 480, QtGui.QImage.Format_RGB888)
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
    window.resize(640, 480)
    window.show()

    app.exec_()

    
    
