# import the necessary packages
from collections import deque
import numpy as np
import argparse
import imutils
import cv2
import math
import serial
import struct
import time
import RPi.GPIO as GPIO


# setup serial connection
ser = serial.Serial('/dev/ttyACM0', 9600)
# let it initialize
time.sleep(2)

#LED
GPIO.setmode(GPIO.BCM)
GPIO.setup(18,GPIO.OUT)


# define the lower and upper boundaries of the "green"
# ball in the HSV color space
purpleLower = (145, 50, 50)
purpleUpper = (170, 255, 255)
redLower7 = (0, 85, 85)
redUpper7 = (7, 255, 255)
redLower177 = (177, 85, 85)
redUpper177 = (179, 255, 255)
 
# initialize the list of tracked points, the frame counter,
# and the coordinate deltas
(x, y) = (0, 0)
(x2, y2) = (0, 0)
 
# if a video path was not supplied, grab the reference
# to the webcam
camera = cv2.VideoCapture(0)
#config camera
camera.set(3,400)
camera.set(4,400)
camera.set(5,20)

 # keep looping
while True:
	GPIO.output(18,GPIO.HIGH)
	# grab the current frame
	(grabbed, frame) = camera.read()
 
	#frame = imutils.resize(frame, width=600)
	blurred = cv2.GaussianBlur(frame, (11, 11), 0)
	hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
	
	maskpurple = cv2.inRange(hsv, purpleLower, purpleUpper)
	maskred1 = cv2.inRange(hsv, redLower177, redUpper177)
	maskred2 = cv2.inRange(hsv, redLower7, redUpper7)
	maskred = cv2.bitwise_or(maskred1, maskred2, mask = None)

	# find contours in the mask and initialize the current
	# (x, y) center of the ball
	cnts = cv2.findContours(maskpurple.copy(), cv2.RETR_EXTERNAL,
		cv2.CHAIN_APPROX_SIMPLE)[-2]
	center = None

	# only proceed if at least one contour was found
	if len(cnts) > 0:
		# find the largest contour in the mask, then use
		# it to compute the minimum enclosing circle and
		# centroid
		c = max(cnts, key=cv2.contourArea)
		((x, y), radius) = cv2.minEnclosingCircle(c)
		M = cv2.moments(c)
		if M["m00"] != 0:
    			center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
 

        #CONTOUR 2
	GPIO.output(18,GPIO.LOW)
	cnts2 = cv2.findContours(maskred.copy(), cv2.RETR_EXTERNAL,
		cv2.CHAIN_APPROX_SIMPLE)[-2]
	center2 = None

	# only proceed if at least one contour was found
	if len(cnts2) > 0:
		# find the largest contour in the mask, then use
		# it to compute the minimum enclosing circle and
		# centroid
		c2 = max(cnts2, key=cv2.contourArea)
		((x2, y2), radius2) = cv2.minEnclosingCircle(c2)
		M2 = cv2.moments(c2)
		if M2["m00"] != 0:
                        center2 = (int(M2["m10"] / M2["m00"]), int(M2["m01"] / M2["m00"]))

	doublecenter = None
	if center != None and center2 != None:
		xdis = center[0] - center2[0]
		ydis = center[1] - center2[1]
		distance = math.sqrt(xdis*xdis + ydis*ydis)
		doublecenter = None
		if ((radius + radius2)*1.25) >= distance:
			doublecenter = (int((center[0]+center2[0])/2), int((center[1]+center2[1])/2))

		if doublecenter != None:
			cv2.putText(frame, "True Center: X: {}, Y: {} Distance: {}".format(doublecenter[0], doublecenter[1], distance),
				(10, frame.shape[0] - 30), cv2.FONT_HERSHEY_SIMPLEX,
				0.35, (0, 0, 255), 1)
			#send x and y coords over serial connection
			#cut x and y by half to fit each in one byte
			Xout = int(doublecenter[0]/2)
			Yout = int(doublecenter[1]/2)
			Zout = int(distance/2)
			# write to arduino as raw binary
			ser.write(struct.pack('>BBB',Xout,Yout,Zout))
	if doublecenter == None:
                ser.write(struct.pack('>BBB',100,100,255))
 
 
# cleanup the camera and close any open windows
camera.release()
cv2.destroyAllWindows()
