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

import threading
import time
from queue import Queue

#Initialise arduino serial comms
arduino = serial.Serial(port="COM9", baudrate=9600, timeout=.1)

def getNoseCoords():
    return [ int(landmarks[mp_pose.PoseLandmark.NOSE.value].x * frameWidth), int(landmarks[mp_pose.PoseLandmark.NOSE.value].y * frameHeight)]

#Thread for serial comms
def serialThreadHandler(coordQueue):
    while threadOpen:
        if not coordQueue.empty():
            coords = coordQueue.get()
            arduino.write(bytes(f"{coords[0]} {coords[1]}" + "\n", "utf-8"))
        time.sleep(0.0001)

#Serial comm thread initialisation
coordQueue = Queue()
serialThread = threading.Thread(target=serialThreadHandler, args=(coordQueue,))
threadOpen = True
serialThread.start()


#Vision model initialisation
mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

#ip_cam = "http://10.0.0.194:8080/video" #IP cam has a lot of latency.
#cap = cv2.VideoCapture(ip_cam)
cap = cv2.VideoCapture(1)

if not cap.isOpened():
    print("Error - could not access camera")

#Camera frame dimensions
frameWidth = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
frameHeight = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
print(frameWidth, frameHeight)

#Camera feed with pose model processing
with mp_pose.Pose(min_detection_confidence=0.9, min_tracking_confidence=0.8) as pose:
    while True:
        ret, frame = cap.read()

        #Frame processing
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        image.flags.writeable = False
        results = pose.process(image)
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        #Draw lines
        cv2.line(img=frame, pt1=(int(frameWidth/2), 0), pt2=(int(frameWidth/2), frameHeight), color=(255, 255, 255), thickness=1, lineType=8, shift=0)
        cv2.line(img=frame, pt1=(0, int(frameHeight/2)), pt2=(frameWidth, int(frameHeight/2)), color=(255, 255, 255), thickness=1, lineType=8, shift=0)

        try:
            landmarks = results.pose_landmarks.landmark
            coords = getNoseCoords()
            coordQueue.put(coords)
            print(f"{coords[0]} {coords[1]}")
            
        except:
            coordQueue.put([0.5, 0.5])
            pass

        mp_drawing.draw_landmarks(frame, results.pose_landmarks, mp_pose.POSE_CONNECTIONS)

        # Display frame
        cv2.imshow('IP Camera Stream', frame)
        
        # Press 'q' to exit the video stream
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    

#Release the capture
cap.release()
cv2.destroyAllWindows()
threadOpen = False