import sys
sys.path.insert(0, "/home/dek/ImageJ")
import numpy as np
import cv2
import MMCorePy


if __name__ == '__main__':
    mmc = MMCorePy.CMMCore()
    mmc.enableStderrLog(True)
    mmc.enableDebugLog(True)

    mmc.loadSystemConfiguration("test.cfg")

    # mmc.setProperty(DEVICE[0], 'PixelType', '32bitRGB')

    cv2.namedWindow('Video')
    mmc.startContinuousSequenceAcquisition(1)

    while True:
        img = mmc.getLastImage()
        if mmc.getRemainingImageCount() > 0:
            img = mmc.getLastImage()
            ret,thresh = cv2.threshold(img,150,255,1)

            contours,h = cv2.findContours(thresh,1,2)
            print len(contours)
            for cnt in contours:
                approx = cv2.approxPolyDP(cnt,0.01*cv2.arcLength(cnt,True),True)
                cv2.drawContours(img,[cnt],0,255,-1)
            cv2.imshow('Video', img)
        else:
            print('No frame')
        if cv2.waitKey(20) >= 0:
            break
    cv2.destroyAllWindows()
    mmc.stopSequenceAcquisition()
    mmc.reset()




cv2.imshow('img',img)
cv2.waitKey(0)
cv2.destroyAllWindows()

