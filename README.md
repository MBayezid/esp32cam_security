# ESP32-CAM Smart Camera System

## 📌 Overview
এই প্রজেক্টটি ESP32-CAM ব্যবহার করে একটি **low-cost smart camera** সিস্টেম তৈরি করার জন্য।  
মূল ফিচার:
- Live HTTP Video Streaming
- Snapshot Capture (JPEG Image)
- Basic Wi-Fi setup (hardcoded credentials)

## 🚀 Features (v1.0.0)
- 📡 Local Wi-Fi connectivity (SSID/Password via code)
- 🎥 HTTP-based MJPEG live stream (accessible in browser)
- 📸 Snapshot button on web interface
- ⚡ Lightweight web server on ESP32

## 🔧 Hardware Required
- ESP32-CAM module
- OV2640 camera
- 5V USB power supply
- (Optional) microSD card >= 32GB 

## 🛠️ Development Setup
1. Install [VS Code](https://code.visualstudio.com/) + PlatformIO
2. Clone repository:
   ```bash
   git clone https://github.com/MBayezid/esp32cam_security.git
   cd eesp32cam_security
