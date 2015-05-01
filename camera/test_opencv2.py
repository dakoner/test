import numpy as np
import cv2

cap = cv2.VideoCapture('my_video-3-noaudio.mkv')

ret, first = cap.read()

while(1):
    ret, frame = cap.read()
    cv2.imshow('frame',frame)
    cv2.waitKey(1)

cap.release()
cv2.destroyAllWindows()
