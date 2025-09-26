// src/main.cpp
// ESP32-CAM wireless MJPEG stream example for PlatformIO (AI-Thinker pinout)
// Replace WIFI_SSID and WIFI_PASS with your network credentials.

#include <WiFi.h>
#include "esp_camera.h"
#include <WebServer.h>

// --------- Replace with your WiFi credentials ----------
const char* WIFI_SSID = "MB";
const char* WIFI_PASS = "mayesha2k10";
// ------------------------------------------------------

WebServer server(80);

// --------- AI-Thinker (ESP32-CAM) default pins ----------
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5

#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22
// ------------------------------------------------------

void initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //init with high specs to get better image quality.
  if(psramFound()){
    config.frame_size = FRAMESIZE_SVGA; // 800x600
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_VGA; // 640x480
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    while(true){
      delay(1000); // stop here if camera init fails
    }
  }
}

String processor(const String& var){
  // simple placeholder processor if you want to inject dynamic HTML later
  if (var == "CAM_STATUS") {
    return "ESP32 CAM READY";
  }
  return String();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'>"
                "<title>ESP32-CAM Stream</title></head><body>"
                "<h2>ESP32-CAM Live Stream</h2>"
                "<img src=\"/stream\" style=\"max-width:100%;height:auto;\">"
                "<p><a href=\"/capture\">Capture single photo</a></p>"
                "</body></html>";
  server.send(200, "text/html", html);
}

void handleCapture() {
  camera_fb_t * fb = esp_camera_fb_get();
  if(!fb) {
    server.send(500, "text/plain", "Camera capture failed");
    return;
  }
  server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
  server.send_P(200, "image/jpeg", (const char *)fb->buf, fb->len);
  esp_camera_fb_return(fb);
}

// Stream MJPEG: multipart/x-mixed-replace
void handleStream() {
  WiFiClient client = server.client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  // server.streamFile(nullptr, ""); // no-op to ensure headers are flushed via server
  client.print(response);

  while (true) {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      delay(100);
      continue;
    }
    client.printf("--frame\r\n");
    client.printf("Content-Type: image/jpeg\r\n");
    client.printf("Content-Length: %u\r\n\r\n", fb->len);
    client.write(fb->buf, fb->len);
    client.printf("\r\n");
    esp_camera_fb_return(fb);

    // if client disconnected, break
    if(!client.connected()){
      break;
    }
    // small delay to be kind to CPU (adjust if you want higher frame-rate)
    delay(50);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting ESP32-CAM Web Server");

  // Init camera
  initCamera();

  // Connect WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.printf("Connecting to %s", WIFI_SSID);
  int wifi_try = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifi_try++;
    if (wifi_try > 60) { // timeout ~30 sec
      Serial.println("\nFailed to connect to WiFi. Restarting...");
      ESP.restart();
    }
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Setup HTTP routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/capture", HTTP_GET, handleCapture);
  // For stream we need a custom handler using sendContent in low-level manner
  server.on("/stream", HTTP_GET, [](){
    handleStream();
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}