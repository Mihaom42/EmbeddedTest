# server.py
import subprocess
import cv2
import numpy as np
import time
from flask import Flask, Response

app = Flask(__name__)

# запускаем C++
proc = subprocess.Popen(["./fake_camera"], stdout=subprocess.PIPE, text=True)

last_time = time.time()
fps = 0

def generate():
    global last_time, fps

    while True:
        line = proc.stdout.readline()
        if not line:
            continue

        parts = line.strip().split()
        if len(parts) != 3:
            continue

        frame_id, x, y = map(int, parts)

        # создаём кадр
        frame = np.zeros((480, 640, 3), dtype=np.uint8)

        # градиентный фон (красиво)
        for i in range(480):
            color = int(255 * i / 480)
            frame[i, :] = (color, 50, 255 - color)

        # движущийся шар
        cv2.circle(frame, (x, y), 20, (0, 255, 0), -1)

        # FPS
        now = time.time()
        fps = 1.0 / (now - last_time)
        last_time = now

        # overlay
        cv2.putText(frame, f"Frame: {frame_id}", (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255,255,255), 2)

        cv2.putText(frame, f"FPS: {fps:.1f}", (10, 60),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0,255,0), 2)

        # шум (как камера)
        noise = np.random.randint(0, 20, frame.shape, dtype=np.uint8)
        frame = cv2.add(frame, noise)

        # кодируем JPEG
        _, jpeg = cv2.imencode('.jpg', frame)

        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' +
               jpeg.tobytes() +
               b'\r\n')

@app.route('/')
def video():
    return Response(generate(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

app.run(host='0.0.0.0', port=8080)