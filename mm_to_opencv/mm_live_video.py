#!/usr/bin/env python2
# -*- coding: utf-8 -*-

"""
Created on 19 Jan. 2014.
@author: Eugene Dvoretsky

Live video acquisition with Micro-Manager adapter and opencv.
"""
import sys
sys.path.insert(0, "/home/dek/ImageJ")
import numpy as np
import cv2
import MMCorePy

# DEVICE = ['Camera', 'DemoCamera', 'DCam']
# DEVICE = ['Camera', 'OpenCVgrabber', 'OpenCVgrabber']
DEVICE = ['Camera', 'video4linux2', 'Video4Linux2']
SHUTTERHUB = ['ArduinoNeoPixel-Hub', 'ArduinoNeoPixel', 'ArduinoNeoPixel-Hub']
SHUTTER = ['ArduinoNeoPixel-Shutter', 'ArduinoNeoPixel', 'ArduinoNeoPixel-Shutter']
# DEVICE = ['Camera', "BaumerOptronic", "BaumerOptronic"]


if __name__ == '__main__':
    mmc = MMCorePy.CMMCore()
    mmc.enableStderrLog(True)
    mmc.enableDebugLog(True)

    mmc.loadSystemConfiguration("test.cfg")

    mmc.setProperty("ArduinoNeoPixel-Shutter", 'Intensity', '25')

    # mmc.setCircularBufferMemoryFootprint(100)
    # mmc.setProperty(DEVICE[0], 'PixelType', '32bitRGB')

    cv2.namedWindow('Video')
    mmc.startContinuousSequenceAcquisition(1)
    # fgbg = cv2.BackgroundSubtractorMOG2()
    fgbg = cv2.BackgroundSubtractorMOG(500, 6, 0.9, 1)

    while True:
        img = mmc.getLastImage()
        # rgb32 = mmc.getLastImage()
        if mmc.getRemainingImageCount() > 0:
            # # rgb32 = mmc.popNextImage()
            # rgb32 = mmc.getLastImage()
            # # Efficient conversion without data copying.
            # # bgr = rgb32.view(dtype=np.uint8).reshape(
            # #     rgb32.shape[0], rgb32.shape[1], 4)[..., :3]
            # cv2.imshow('Video', bgr)

            img = mmc.getLastImage()
            # cv2.imshow('Video', img)
            fgmask = fgbg.apply(img)
            draw = img & fgmask
            cv2.imshow('Video', draw)
            
        else:
            print('No frame')
        if cv2.waitKey(20) >= 0:
            break
    cv2.destroyAllWindows()
    mmc.stopSequenceAcquisition()
    mmc.reset()
