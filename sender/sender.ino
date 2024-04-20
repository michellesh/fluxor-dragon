// clang-format off
#include <WiFi.h>
#include <FastLED.h>
#include <esp_now.h>

#include "fluxor-dragon-shared.h"
// clang-format on

#define KNOB_SPEED 13
#define KNOB_COLOR_LEFT 27
#define KNOB_COLOR_RIGHT 26

#define BUTTON_FLASH 19
#define BUTTON_LASERS 21
#define BUTTON_TWINKLE 22
#define BUTTON_WINDSHIELD 23

struct Button {
  int pin;
  char description[20];
  bool pressed;
};

struct Knob {
  int pin;
  char description[20];
  int value;
  int prev;
};

// Knob Actions
msg colorLeft = {ACTION_SET_COLOR_LEFT};
msg colorRight = {ACTION_SET_COLOR_RIGHT};
msg speed = {ACTION_SPEED};
msg data;

// Button Actions
msg background = {ACTION_SET_BACKGROUND, DEFAULT_VIZ};
msg strobeOff = {ACTION_STROBE_OFF};
msg strobeOn = {ACTION_STROBE_ON};

Button buttonLasers = {BUTTON_LASERS, "LASERS", false};
Button buttonTwinkle = {BUTTON_TWINKLE, "TWINKLE", false};
Button buttonWindshield = {BUTTON_WINDSHIELD, "WINDSHIELD", false};
Button buttonFlash = {BUTTON_FLASH, "FLASH", false};

Knob knobColorLeft = {KNOB_COLOR_LEFT, "COLOR LEFT"};
Knob knobColorRight = {KNOB_COLOR_RIGHT, "COLOR RIGHT"};
Knob knobSpeed = {KNOB_SPEED, "SPEED"};

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // register peer
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // register first peer
  memcpy(peerInfo.peer_addr, receiverAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // register second peer
  memcpy(peerInfo.peer_addr, receiverAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(BUTTON_FLASH, INPUT);
  pinMode(BUTTON_LASERS, INPUT);
  pinMode(BUTTON_TWINKLE, INPUT);
  pinMode(BUTTON_WINDSHIELD, INPUT);
}

void loop() {
  static bool testStrobe = true;
  EVERY_N_SECONDS(1) {
    if (testStrobe) {
      send(strobeOn);
    } else {
      send(strobeOff);
    }
    testStrobe = !testStrobe;
  }

  int buttonReadFlash = digitalRead(BUTTON_FLASH);
  int buttonReadLasers = digitalRead(BUTTON_LASERS);
  int buttonReadTwinkle = digitalRead(BUTTON_TWINKLE);
  int buttonReadWindshield = digitalRead(BUTTON_WINDSHIELD);

  int knobReadColorLeft = analogRead(KNOB_COLOR_LEFT);
  int knobReadColorRight = analogRead(KNOB_COLOR_RIGHT);
  int knobReadSpeed = analogRead(KNOB_SPEED);

  EVERY_N_MILLISECONDS(1000) {
    Serial.print("buttonReadFlash: ");
    Serial.println(buttonReadFlash);
    Serial.print("buttonReadLasers: ");
    Serial.println(buttonReadLasers);
    Serial.print("buttonReadTwinkle: ");
    Serial.println(buttonReadTwinkle);
    Serial.print("buttonReadWindshield: ");
    Serial.println(buttonReadWindshield);
    Serial.println("---");
    Serial.print("knobReadColorLeft: ");
    Serial.println(knobReadColorLeft);
    Serial.print("knobReadColorRight: ");
    Serial.println(knobReadColorRight);
    Serial.print("knobReadSpeed: ");
    Serial.println(knobReadSpeed);
    Serial.println();
  }
}
