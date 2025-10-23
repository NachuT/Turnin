#include <WiFi.h>
#include <esp_now.h>
#include "WS_Flow.h"

char Text[3] = "->";           
char scrollDirection[6] = "";  
char lastDirection[6] = "";    

typedef struct struct_message {
  char direction[8];
} struct_message;

struct_message incomingMessage;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));
  strncpy(scrollDirection, incomingMessage.direction, sizeof(scrollDirection) - 1);
  scrollDirection[sizeof(scrollDirection) - 1] = '\0';

  if (strcmp(scrollDirection, lastDirection) != 0) {
    if (strcmp(scrollDirection, "left") == 0) {
      strcpy(Text, "<-");
    } else if (strcmp(scrollDirection, "right") == 0) {
      strcpy(Text, "->");
    }
    strcpy(lastDirection, scrollDirection);  
    Serial.print("Direction updated to: ");
    Serial.println(scrollDirection);
  } else {

    strcpy(Text, "");
    strcpy(lastDirection, "");  
    Serial.println("Repeated direction, clearing text and resetting");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Matrix_Init();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Receiver ready!");
}

void loop() {
  Text_Flow(Text, scrollDirection);  
  delay(25);
}
