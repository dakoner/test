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
        # cv2.destroyAllWindows()
        self.mmc.stopSequenceAcquisition()
        self.mmc.reset()

        
class MainWindow(QtGui.QMainWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)


        self.mm = MM()
        self.mm.start()

        image = cv2.cvtColor(self.mm.getImage(), cv2.COLOR_GRAY2RGB)
        self.image = QtGui.QImage(image.data, 640, 480, QtGui.QImage.Format_RGB888)
        # self.image.fill(QtGui.qRgb(255,0,0))
        self.pix = QtGui.QPixmap.fromImage(self.image)

        self.lbl = QtGui.QLabel(self)
        self.lbl.setPixmap(self.pix)
        self.lbl.show()
        self.setCentralWidget(self.lbl)

        self.timer = QtCore.QTimer()
        self.timer.start(.04)
        self.connect(self.timer, QtCore.SIGNAL('timeout()'), self.updateData) 


    def updateData(self):
        image = cv2.cvtColor(self.mm.getImage(), cv2.COLOR_GRAY2RGB)
        self.image = QtGui.QImage(image.data, 640, 480, QtGui.QImage.Format_RGB888)
        self.pix.convertFromImage(self.image)
        self.repaint()
        # self.lbl.setPixmap(self.pix)
    
                        
if __name__ == '__main__':
    app = QtGui.QApplication([])
    window = MainWindow()
    window.resize(640, 480)
    window.show()

    app.exec_()

    
    
