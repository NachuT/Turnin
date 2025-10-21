#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
  char direction[8];
} struct_message;

struct_message outgoingMessage;
uint8_t broadcastAddress[] = {0x50, 0x78, 0x7D, 0x18, 0x7B, 0x64};
#define LEFT_BUTTON 36
#define RIGHT_BUTTON 7
bool lastLeftState = HIGH;
bool lastRightState = HIGH;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Ready. Press LEFT or RIGHT buttons.");
}

void loop() {
  bool currentLeftState = digitalRead(LEFT_BUTTON);
  bool currentRightState = digitalRead(RIGHT_BUTTON);

  if (lastLeftState == HIGH && currentLeftState == LOW) {
    strcpy(outgoingMessage.direction, "left");
    esp_now_send(broadcastAddress, (uint8_t *)&outgoingMessage, sizeof(outgoingMessage));
    Serial.println("Sent: left");
    delay(200);
  }

 
  if (lastRightState == HIGH && currentRightState == LOW) {
    strcpy(outgoingMessage.direction, "right");
    esp_now_send(broadcastAddress, (uint8_t *)&outgoingMessage, sizeof(outgoingMessage));
    Serial.println("Sent: right");
    delay(200);
  }

  lastLeftState = currentLeftState;
  lastRightState = currentRightState;
}
