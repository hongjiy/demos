# import the necessary packages
from collections import deque
import numpy as np
import argparse
import imutils
import cv2

camera = cv2.VideoCapture(0)
#config camera
camera.set(3,640)
camera.set(4,480)
camera.set(5,20)

# keep looping
while True:
	# grab the current frame
	(grabbed, frame) = camera.read()

	# show the frame to our screen and increment the frame counter
	cv2.imshow("Frame", frame)
	key = cv2.waitKey(1) & 0xFF
 
	# if the 'q' key is pressed, stop the loop
	if key == ord("q"):
		break
 
# cleanup the camera and close any open windows
camera.release()
cv2.destroyAllWindows()
