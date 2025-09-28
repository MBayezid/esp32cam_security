# ESP32-CAM Security (edge TinyML)

Simple open-source security camera that runs on an AI-Thinker ESP32-CAM. The project streams MJPEG video, serves single-frame captures, and is designed to run TinyML/TensorFlow Lite Micro inference on-device to minimize cloud usage and protect privacy.

How it works
- Firmware entry: src/main.cpp — initializes the camera, connects to Wi‑Fi, and exposes HTTP endpoints:
  - GET / — simple status page
  - GET /stream — live MJPEG stream
  - GET /capture — single JPEG capture
- Edge inference (planned): capture frames locally and run a TFLite Micro model to detect objects/people; trigger local alerts or optional opt‑in reporting.

Quick start
- Edit WIFI_SSID and WIFI_PASS in src/main.cpp (or use planned runtime config).
- Build & upload with PlatformIO on Windows.
- Open the device IP shown on serial console and visit / to see the stream.

Where to read more
- overview.md — high level architecture and usage examples.
- planing.md — development roadmap and tasks.
- PRD.md — product requirements and goals.

Goals
- Keep inference and storage local by default.
- Provide a lightweight, privacy-first edge security camera.
- Make it easy to extend models, notifications (MQTT/local GPIO), and secure OTA/model updates.

Contributing
- Follow the files above for plans and requirements before changing core behavior.