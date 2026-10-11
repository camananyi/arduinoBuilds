import cv2
import mediapipe as mp
import math
import serial
import time

arduino = serial.Serial('/dev/cu.usbmodem64E8335D23102', 9600)
time.sleep(3)
last_volume = -1

mp_hands = mp.solutions.hands
mp_draw = mp.solutions.drawing_utils
hands = mp_hands.Hands(max_num_hands=1, min_detection_confidence=0.7)

camera = cv2.VideoCapture(1)

while True:
    success, frame = camera.read()
    frame = cv2.flip(frame, 1)
    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = hands.process(rgb)

    if results.multi_hand_landmarks:
        for hand in results.multi_hand_landmarks:
            

            h, w, _ = frame.shape
            thumb = hand.landmark[4]
            index = hand.landmark[8]
            x1, y1 = int(thumb.x * w), int(thumb.y * h)
            x2, y2 = int(index.x * w), int(index.y * h)

            tips = [8, 12, 16, 20]
            knuckles = [6, 10, 14, 18]
            fingers_up = 0

            for i in range(4):
                if hand.landmark[tips[i]].y < hand.landmark[knuckles[i]].y:
                    fingers_up = fingers_up + 1

            if fingers_up == 0:
                hand_state = "MUTE"
                volume = 0
            else:
                hand_state = "OPEN"
                distance = math.hypot(x2 - x1, y2 - y1)
                volume = (distance - 20) / (370 - 20) * 50
                volume = int(max(0, min(50, volume)))

            if volume != last_volume:
                arduino.write((str(volume) + '\n').encode())
                last_volume = volume

            cv2.putText(frame, "Volume: " + str(volume), (20, 50),
                        cv2.FONT_HERSHEY_SIMPLEX, 1.2, (255, 255, 255), 3)
            cv2.putText(frame, hand_state, (20, 100),
                        cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 255, 255), 3)

    cv2.imshow("BOOMBOX", frame)
    if cv2.waitKey(1) == ord('q'):
        break

camera.release()
cv2.destroyAllWindows()
arduino.close()
