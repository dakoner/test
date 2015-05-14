import copy
from PySide import QtGui, QtCore
import time
import sys
import numpy as np
import cv2



class MainWindow(QtGui.QMainWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)

        self.cam = cv2.VideoCapture(0)
        ret, self.prev = self.cam.read()
        # self.cam = cv2.VideoCapture('/home/dek/my_video-6.mkv')
        self.width = long(self.cam.get(cv2.cv.CV_CAP_PROP_FRAME_WIDTH))
        self.height = long(self.cam.get(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT))
        self.cam.set(3, self.width)
        self.cam.set(4, self.height)
        self.pix = QtGui.QPixmap(self.width, self.height)
        self.pix2 = QtGui.QPixmap(self.width, self.height)

        self.lbl = QtGui.QLabel(self)
        self.lbl.setPixmap(self.pix)
        self.lbl.show()

        self.lbl2 = QtGui.QLabel(self)
        self.lbl2.setPixmap(self.pix2)
        self.lbl2.show()


        self.widget = QtGui.QWidget(self)
        self.setCentralWidget(self.widget)

        self.vbox = QtGui.QVBoxLayout()
        self.widget.setLayout(self.vbox)    
        self.vbox.addStretch(1)
        self.vbox.addWidget(self.lbl)
        self.vbox.addWidget(self.lbl2)
       

        self.timer = QtCore.QTimer()
        self.timer.start(.01)
        self.connect(self.timer, QtCore.SIGNAL('timeout()'), self.updateData) 

        self.fgbg = cv2.BackgroundSubtractorMOG(500, 6, 0.9, 1)
        # self.fgbg = cv2.BackgroundSubtractorMOG2()

        self.frame = 0
        self.resize(self.width, self.height*2)
        self.show()
        
## TODO(dek): run at shutdown
#        cam.release()
#        cv2.destroyAllWindows()

    def updateData(self):
        ret, img = self.cam.read()
        if ret:
            self.image = QtGui.QImage(img.data, self.width, self.height, QtGui.QImage.Format_RGB888).rgbSwapped()

            # p = cv2.cvtColor(self.prev, cv2.COLOR_BGR2GRAY)
            i = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            # flow = cv2.calcOpticalFlowFarneback(p, i, 0.5, 3, 15, 3, 5, 1.2, 0)
            # w, h = self.width, self.height
            # step = 16
            # y, x = np.mgrid[step/2:h:step, step/2:w:step].reshape(2,-1)
            # fx, fy = flow[y,x].T
            # polys = np.vstack([x, y, x+fx, y+fy]).T.reshape(-1, 2, 2)
            # polys = np.int32(polys + 0.5)
            # painter = QtGui.QPainter(self.image)
            # blue = QtGui.QColor()
            # blue.setNamedColor("blue")
            # painter.setPen(QtGui.QPen(blue))
            # for lines in polys:
            #     p1 = QtCore.QPoint(*lines[0])
            #     p2 = QtCore.QPoint(*lines[1])
            #     l = QtCore.QLine(p1, p2)
            #     painter.drawLine(l)

            fgmask = self.fgbg.apply(i)
            fgmask  = cv2.cvtColor(fgmask, cv2.COLOR_GRAY2BGR)
            # draw = img & fgmask
            self.prev = img
            self.image2 = QtGui.QImage(fgmask.data, self.width, self.height, QtGui.QImage.Format_RGB888).rgbSwapped()



            # self.image.save("frame.%05d.jpg" % self.frame)
            self.frame += 1


            self.pix.convertFromImage(self.image) 
            self.pix2.convertFromImage(self.image2) 
            self.repaint()
        else:
            print "failed to get frame"
    
                        
if __name__ == '__main__':
    app = QtGui.QApplication([])
    window = MainWindow()

    app.exec_()

    
    
