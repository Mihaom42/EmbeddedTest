# server.py
import subprocess
import cv2
import os
import sys
import numpy as np
import time
from flask import Flask, Response

app = Flask(__name__)

if getattr(sys, 'frozen', False):
    base_dir = os.path.dirname(sys.executable)
else:
    base_dir = os.path.dirname(os.path.abspath(__file__))

exe_path = os.path.join(base_dir, "videoStreamer.exe")

proc = subprocess.Popen(
    [exe_path],
    stdout=subprocess.PIPE,
    text=True
)

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

        frame = np.zeros((480, 640, 3), dtype=np.uint8)

        for i in range(480):
            color = int(255 * i / 480)
            frame[i, :] = (color, 50, 255 - color)

        cv2.circle(frame, (x, y), 20, (0, 255, 0), -1)

        now = time.time()
        dt = now - last_time

        fps = 0.9 * fps + 0.1 * (1.0 / dt if dt > 0 else 0)

        last_time = now

        cv2.putText(frame, f"Frame: {frame_id}", (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255,255,255), 2)

        cv2.putText(frame, f"FPS: {fps:.1f}", (10, 60),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0,255,0), 2)

        noise = np.random.randint(0, 20, frame.shape, dtype=np.uint8)
        frame = cv2.add(frame, noise)

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