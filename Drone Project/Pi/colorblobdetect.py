# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import imutils
import numpy as np
import time
import cv2


# define the red boundary
boundary = [([0, 0, 100], [50, 50, 255])]

# Setup SimpleBlobDetector parameters.
params = cv2.SimpleBlobDetector_Params()
# Change thresholds
params.minThreshold = 10;
params.maxThreshold = 200;
# Distance Between Blobs
params.minDistBetweenBlobs = 100
# Filter by Area.
params.filterByArea = False
params.minArea = 25
params.maxArea = 4000000
# Filter by Circularity
params.filterByCircularity = False
params.minCircularity = 0.1
# Filter by Convexity
params.filterByConvexity = False
params.minConvexity = 0.87
# Filter by Inertia
params.filterByInertia = False
params.minInertiaRatio = 0.01

#define blob detector
detector = cv2.SimpleBlobDetector_create(params)

# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 20
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

        #greyscale
        gray = cv2.cvtColor(output, cv2.COLOR_BGR2GRAY)

        #blur and threshold
        blurred = cv2.GaussianBlur(gray, (5, 5), 0)
        thresh = cv2.threshold(blurred, 30, 255, cv2.THRESH_BINARY_INV)[1]

        #find blobs
        keypoints = detector.detect(thresh)

        #draw kepoints on blobs
        # Draw detected blobs as green circles.
        # cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle corresponds to the size of blob
        im_with_keypoints = cv2.drawKeypoints(thresh, keypoints, np.array([]), (0,255,0), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

        #show image
        cv2.imshow("Image", im_with_keypoints)
        key = cv2.waitKey(1) & 0xFF
 
	# clear the stream in preparation for the next frame
        rawCapture.truncate(0)
 
	# if the `q` key was pressed, break from the loop
        if key == ord("q"):
                break
