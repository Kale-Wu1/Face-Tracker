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

def getNoseCoords():
    return [landmarks[mp_pose.PoseLandmark.NOSE.value].x, landmarks[mp_pose.PoseLandmark.NOSE.value].y]

#Vision model initialisation
mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

ip_cam = "http://10.0.0.194:8080/video" #IP cam has a lot of latency.
cap = cv2.VideoCapture(ip_cam)

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
            print(f"Nose at {coords[0]}, {coords[1]}.")
            
            #Targeting
            if(coords[0] <= 0.4):
                print("LEFT")
            elif(coords[0] >= 0.6):
                print("RIGHT")
            else:
                print("X CENTERED")

            if(coords[1] <= 0.4):
                print("UP")
            elif(coords[1] >= 0.6):
                print("DOWN")
            else:
                print("Y CENTERED")
            
            
        except:
            print("No face detected")
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