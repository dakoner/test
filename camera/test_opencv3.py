from subprocess import call
import numpy as np
import cv2


# cap = cv2.VideoCapture('my_video-3-noaudio.mkv')
cap = cv2.VideoCapture(0)

fgbg = cv2.BackgroundSubtractorMOG2()
i = 238
first = True
framenum = 0
while(cap.isOpened()):
  ret, frame = cap.read()
  if framenum % 10 == 0:
    call(["v4l2-ctl", "-d", "/dev/video0", "-c", "focus_auto=0", "-c", "focus_absolute="+str(i)])
  if frame is not None:
    fgmask = fgbg.apply(frame)
    mask_rbg = cv2.cvtColor(fgmask,cv2.COLOR_GRAY2BGR)
    draw = frame & mask_rbg

    cv2.imshow('frame',draw)
    # cv2.imshow('frame',frame)
    k = cv2.waitKey(30)
    if k == 27:
      break
    if k == 97:
      i = i + 17
      print i
  else:
    print ret
  first = False
  framenum += 1
cap.release()
cv2.destroyAllWindows()
