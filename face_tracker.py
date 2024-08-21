'''
Goal: Detect and point camera centering the closest detected face in frame with optimised movement

1. Open camera - Done
2. Detect face in camera
3. Correct if not centered by sending motor control info to arduino
'''

import cv2

import mediapipe as mp
import numpy as np
import serial

#Initialise arduino serial comms
arduino = serial.Serial(port="COM9", baudrate=9600, timeout=.1)

#Function for serial communication to arduino
def serialWrite(data):
    arduino.write(bytes(data + "\n", "utf-8"))

#Vision model initialisation
mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

ip_cam = "http://10.0.0.194:8080/video"
cap = cv2.VideoCapture(ip_cam)

if not cap.isOpened():
    print("Error - could not access camera")

#Camera feed with pose model processing
with mp_pose.Pose(min_detection_confidence=0.9, min_tracking_confidence=0.8) as pose:
    while True:
        ret, frame = cap.read()

        # Display the resulting frame
        cv2.imshow('IP Camera Stream', frame)

        results = pose.process(frame)

        # Press 'q' to exit the video stream
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    

#Release the capture
cap.release()
cv2.destroyAllWindows()