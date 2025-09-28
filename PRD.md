# ESP32-CAM Security Camera — Product Requirements Document (PRD)

## Overview
An open-source, on-edge security camera that performs primary recognition locally (TinyML / TensorFlow Lite Micro) to minimize cloud processing, preserve privacy, and reduce bandwidth. The reference firmware in [src/main.cpp](src/main.cpp) implements camera initialization, a simple HTTP server and MJPEG streaming.

## Goals
- Provide reliable local video capture and lightweight on-device inference for object/person detection.
- Stream live video and serve single-frame captures via HTTP with minimal latency.
- Keep user data private by default: no cloud upload unless explicitly enabled.

## Key References
- Firmware and HTTP endpoints: [src/main.cpp](src/main.cpp) — see [`initCamera`](src/main.cpp), [`handleStream`](src/main.cpp), [`handleCapture`](src/main.cpp), [`setup`](src/main.cpp), [`loop`](src/main.cpp).
- Build configuration: [platformio.ini](platformio.ini).
- Project layout and library guidance: [include/README](include/README), [lib/README](lib/README), [test/README](test/README).

## Target Users
- Hobbyists and DIY home-security users.
- Privacy-conscious users and small businesses needing edge inference.
- Developers contributing models or integrations.

## Scope & Features (MVP)
- Hardware: AI-Thinker ESP32-CAM (as configured in [src/main.cpp](src/main.cpp) pin mapping).
- Camera capture and MJPEG streaming endpoint (`/stream`) — implemented in [`handleStream`](src/main.cpp).
- Single-photo capture endpoint (`/capture`) — implemented in [`handleCapture`](src/main.cpp).
- On-device inference pipeline:
  - Load TensorFlow Lite Micro model from SPIFFS/flash.
  - Run periodic inference on frames (configurable frame-skip).
  - Emit local events when inference triggers (e.g., person detected).
- Local UI (web page) showing stream and simple status (root handler in [src/main.cpp](src/main.cpp)).
- Configurable Wi-Fi via build-time or runtime method (current example uses constants in code).

## Performance & Constraints
- Target frame resolution: fallback to VGA if PSRAM absent (as in current code path).
- Aim for inference latency < 200 ms for classification on single frame using optimized TFLM models.
- Keep RAM usage under ESP32 constraints; leverage PSRAM when available (already checked in [src/main.cpp](src/main.cpp)).

## Privacy & Security
- Default: inference and storage stay local. No cloud upload unless user opts in.
- Secure OTA and model updates via signed packages (future).
- Minimal exposed endpoints: `/`, `/stream`, `/capture`. Add authentication for remote access in later iterations.

## Software Architecture (high level)
- Camera capture module: camera init and frame buffer handling — [`initCamera`](src/main.cpp).
- Web server: lightweight HTTP server serving HTML, MJPEG stream, and capture endpoint — routes defined in [`setup`](src/main.cpp).
- Inference module: TensorFlow Lite Micro runner (new component), taking frames from camera FB, producing detections.
- Event manager: local notification (GPIO buzzer/LED) and optional push to cloud or local MQTT.

## API / Endpoints
- GET / — simple status page (implemented in [src/main.cpp](src/main.cpp)).
- GET /stream — MJPEG stream (handled by [`handleStream`](src/main.cpp)).
- GET /capture — single JPEG image (handled by [`handleCapture`](src/main.cpp)).
- Future: POST /config, GET /status, POST /model-upload (secure).

## Testing & Validation
- Unit tests and integration guidelines: see [test/README](test/README).
- Validate camera init across variants (PSRAM/no-PSRAM).
- Validate inference detection precision, false-positive rate, and performance budget on-device.

## Milestones & Roadmap
1. Stabilize camera streaming and capture (current code in [src/main.cpp](src/main.cpp)).
2. Integrate TinyML inference pipeline (TFLite Micro) and basic person detector on-device.
3. Add configuration UI and secure access controls.
4. Model update/management and signed OTA/model delivery.
5. Optional cloud integration (opt-in) for alerts and longer-term storage.

## Build & Contribution
- Build with PlatformIO: see [platformio.ini](platformio.ini).
- Follow library layout guidance in [lib/README](lib/README) and headers guidance in [include/README](include/README).
- Tests: use PlatformIO Test Runner as described in [test/README](test/README).

## Success Metrics
- On-device inference operating within target latency and memory budget.
- Accurate detection (target precision/recall thresholds to be defined per model).
- Low false positives during continuous operation.
- Secure, private-by-default behavior with clear opt-in for cloud features.

Contact/contribution guidelines, licensing, and detailed acceptance criteria should be added in subsequent revisions.