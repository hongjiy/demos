# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import imutils
import numpy as np
import time
import cv2
from shapedetect import ShapeDetector

# define the red boundary
boundary = [([0, 0, 100], [50, 50, 255])]

# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 30
rawCapture = PiRGBArray(camera, size=(640, 480))
 
# allow the camera to warmup
time.sleep(0.1)
 
# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
        # grab the raw NumPy array representing the image, then initialize the timestamp
        # and occupied/unoccupied text
        image = frame.array

        # loop over the boundaries
        for (lower, upper) in boundary:
                # create NumPy arrays from the boundaries
                lower = np.array(lower, dtype = "uint8")
                upper = np.array(upper, dtype = "uint8")
 
        # find the colors within the specified boundaries and apply
        # the mask
        mask = cv2.inRange(image, lower, upper)
        output = cv2.bitwise_and(image, image, mask = mask)

        # resize it to a smaller factor so that
        # the shapes can be approximated better
        resized = imutils.resize(output, width=300)
        ratio = image.shape[0] / float(resized.shape[0])
 
        # convert the resized image to grayscale, blur it slightly,
        # and threshold it
        gray = cv2.cvtColor(resized, cv2.COLOR_BGR2GRAY)
        blurred = cv2.GaussianBlur(gray, (5, 5), 0)
        thresh = cv2.threshold(blurred, 60, 255, cv2.THRESH_BINARY)[1]
 
        # find contours in the thresholded image and initialize the
        # shape detector
        cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE)
        cnts = cnts[0] if imutils.is_cv2() else cnts[1]
        sd = ShapeDetector()

        # loop over the contours
        for c in cnts:
                # compute the center of the contour, then detect the name of the
                # shape using only the contour
                M = cv2.moments(c)
                cX = 100#int((M["m10"] / M["m00"]) * ratio)
                cY = 100#int((M["m01"] / M["m00"]) * ratio)
                shape = sd.detect(c)
 
                # multiply the contour (x, y)-coordinates by the resize ratio,
                # then draw the contours and the name of the shape on the image
                c = c.astype("float")
                c *= ratio
                c = c.astype("int")
                cv2.drawContours(output, [c], -1, (0, 255, 0), 2)
                cv2.putText(output, shape, (cX, cY), cv2.FONT_HERSHEY_SIMPLEX,
                0.5, (255, 255, 255), 2)
 
                # show the output image
                #cv2.imshow("Image", image)
                #key = cv2.waitKey(1) & 0xFF

        cv2.imshow("Image", output)
        key = cv2.waitKey(1) & 0xFF
 
        # show the images
        #cv2.imshow("images", np.hstack([image, output]))
        #key = cv2.waitKey(1) & 0xFF
 
	# clear the stream in preparation for the next frame
        rawCapture.truncate(0)
 
	# if the `q` key was pressed, break from the loop
        if key == ord("q"):
                break
