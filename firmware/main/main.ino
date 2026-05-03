#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>

const char* ssid = "ssid";
const char* password = "pass";

const int BRC_PIN = 14; 
const int LED_PIN = 2;

SoftwareSerial roombaRead(13, -1); 

WebSocketsServer webSocket = WebSocketsServer(81);

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_BIN) {
    if (length == 1 && payload[0] == 170) {
      digitalWrite(BRC_PIN, LOW); 
      delay(500); 
      digitalWrite(BRC_PIN, HIGH);
      delay(1000); 
      Serial.write(128);
    } else {
      Serial.write(payload, length);
    }
  }
}

void setup() {
  pinMode(BRC_PIN, OUTPUT);
  digitalWrite(BRC_PIN, HIGH);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200); 
  roombaRead.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
  digitalWrite(LED_PIN, LOW);

  MDNS.begin("roomba");
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {
  webSocket.loop();
  MDNS.update();
  if (roombaRead.available()) {
    size_t len = roombaRead.available();
    uint8_t buf[len];
    roombaRead.readBytes(buf, len);
    webSocket.broadcastBIN(buf, len);
  }
}