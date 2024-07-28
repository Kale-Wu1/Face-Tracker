import cv2

ip_cam = "http://[IP not uploaded]/video"

cap = cv2.VideoCapture(ip_cam)

if not cap.isOpened():
    print("Error - could not access camera")


while True:
    ret, frame = cap.read()

    # Display the resulting frame
    cv2.imshow('IP Camera Stream', frame)

    

    # Press 'q' to exit the video stream
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

#Release the capture
cap.release()
cv2.destroyAllWindows()