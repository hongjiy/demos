# import the necessary packages
from collections import deque
import numpy as np
import argparse
import imutils
import cv2
import math
import RPi.GPIO as GPIO

#setup GPIO
GPIO.setmode(GPIO.BOARD)

#channel_list = [29, 31, 33, 35]
GPIO.setup(29, GPIO.OUT)
GPIO.setup(31, GPIO.OUT)
GPIO.setup(33, GPIO.OUT)
GPIO.setup(35, GPIO.OUT)


 
# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",
	help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=32,
	help="max buffer size")
args = vars(ap.parse_args())

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
#(GPIOx, GPIOy) = ([0,1], [0,1])
 
# if a video path was not supplied, grab the reference
# to the webcam
if not args.get("video", False):
	camera = cv2.VideoCapture(0)
	#config camera
	camera.set(3,400)
	camera.set(4,400)
	camera.set(5,20)
 
# otherwise, grab a reference to the video file
else:
	camera = cv2.VideoCapture(args["video"])

# keep looping
while True:
	# grab the current frame
	(grabbed, frame) = camera.read()
 
	# if we are viewing a video and we did not grab a frame,
	# then we have reached the end of the video
	if args.get("video") and not grabbed:
		break
 
	# resize the frame, blur it, and convert it to the HSV
	# color space
	#frame = imutils.resize(frame, width=600)
	blurred = cv2.GaussianBlur(frame, (11, 11), 0)
	hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
 
	# construct a mask for the color "green", then perform
	# a series of dilations and erosions to remove any small
	# blobs left in the mask
	
	#mask = cv2.erode(mask, None, iterations=2)
	#mask = cv2.dilate(mask, None, iterations=2)
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
 
		# only proceed if the radius meets a minimum size
		if radius > 3:
			# draw the circle and centroid on the frame,
			# then update the list of tracked points
			cv2.circle(frame, (int(x), int(y)), int(radius),
				(0, 255, 255), 2)
			cv2.circle(frame, center, 5, (0, 0, 255), -1)

        #CONTOUR 2
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
 
		# only proceed if the radius meets a minimum size
		if radius2 > 1:
			# draw the circle and centroid on the frame,
			# then update the list of tracked points
			cv2.circle(frame, (int(x2), int(y2)), int(radius2),
				(0, 255, 255), 2)
			cv2.circle(frame, center2, 5, (0, 0, 255), -1)

 
	if center != None:
		cv2.putText(frame, "CoMPurple: X: {}, Y: {}".format(center[0], center[1]),
			(10, frame.shape[0] - 10), cv2.FONT_HERSHEY_SIMPLEX,
			0.35, (0, 0, 255), 1)

	if center2 != None:
		cv2.putText(frame, "CoMRed: X: {}, Y: {}".format(center2[0], center2[1]),
			(150, frame.shape[0] - 10), cv2.FONT_HERSHEY_SIMPLEX,
			0.35, (0, 0, 255), 1)

	if center != None and center2 != None:
		xdis = center[0] - center2[0]
		ydis = center[1] - center2[1]
		distance = math.sqrt(xdis*xdis + ydis*ydis)
		doublecenter = None
		if ((radius + radius2)*1.25) >= distance:
			doublecenter = (int((center[0]+center2[0])/2), int((center[1]+center2[1])/2))


			#Set GPIO X
			if doublecenter[0] <= (400/3):
                                GPIO.output(29, GPIO.LOW)
                                GPIO.output(31, GPIO.LOW)
			if doublecenter[0] > (400/3) and doublecenter[0] < (800/3):
                                GPIO.output(29, GPIO.LOW)
                                GPIO.output(31, GPIO.HIGH)
			if doublecenter[0] >= (800/3):
                                GPIO.output(29, GPIO.HIGH)
                                GPIO.output(31, GPIO.LOW)
			#Set GPIO Y
			if doublecenter[1] <= (400/3):
                                GPIO.output(33, GPIO.LOW)
                                GPIO.output(35, GPIO.LOW)
			if doublecenter[1] > (400/3) and doublecenter[1] < (800/3):
                                GPIO.output(33, GPIO.LOW)
                                GPIO.output(35, GPIO.HIGH)
			if doublecenter[1] >= (800/3):
                                GPIO.output(33, GPIO.HIGH)
                                GPIO.output(35, GPIO.LOW)
 

		if doublecenter != None:
			cv2.putText(frame, "True Center: X: {}, Y: {}".format(doublecenter[0], doublecenter[1]),
				(10, frame.shape[0] - 30), cv2.FONT_HERSHEY_SIMPLEX,
				0.35, (0, 0, 255), 1)
 
	# show the frame to our screen and increment the frame counter
	cv2.imshow("Frame", frame)
	cv2.imshow("maskpurple", maskpurple)
	cv2.imshow("maskred", maskred)
	key = cv2.waitKey(1) & 0xFF
 
	# if the 'q' key is pressed, stop the loop
	if key == ord("q"):
		break
 
# cleanup the camera and close any open windows
camera.release()
cv2.destroyAllWindows()
GPIO.cleanup()
