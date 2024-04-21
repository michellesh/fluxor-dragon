// clang-format off
#include <WiFi.h>
#include <FastLED.h>
#include <esp_now.h>

#include "Scale.h"

#include "fluxor-dragon-shared.h"
// clang-format on

// Knob pins. Works: 34, 35, 32, 33
#define KNOB_SPEED 33
#define KNOB_COLOR_LEFT 34
#define KNOB_COLOR_RIGHT 35
#define KNOB_COLOR_EYE 32 // 0 -> 4095

// Button pins. Works: 23, 22, 2, 4
#define BUTTON_FLASH 2
#define BUTTON_LASERS 4
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
msg colorEye = {ACTION_SET_COLOR_EYE};
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

Knob knobColorEye = {KNOB_COLOR_EYE, "COLOR EYE"};
Knob knobColorLeft = {KNOB_COLOR_LEFT, "COLOR LEFT"};
Knob knobColorRight = {KNOB_COLOR_RIGHT, "COLOR RIGHT"};
Knob knobSpeed = {KNOB_SPEED, "SPEED"};

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  /*
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  */
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
  /*
  // TEST STROBE
  static bool testStrobe = true;
  EVERY_N_SECONDS(1) {
    if (testStrobe) {
      send(strobeOn);
    } else {
      send(strobeOff);
    }
    testStrobe = !testStrobe;
  }
  */

  // TEST CYCLE BACKGROUND
  static int testCycleBackground = 1;
  EVERY_N_SECONDS(1) {
    background.value = testCycleBackground;
    send(background);
    testCycleBackground++;
    if (testCycleBackground > 3) {
      testCycleBackground = 1;
    }
  }

  checkKnobChanged(knobColorEye);
  checkKnobChanged(knobColorLeft);
  checkKnobChanged(knobColorRight);
  checkKnobChanged(knobSpeed);

  checkButtonPressed(buttonLasers);
  checkButtonPressed(buttonTwinkle);
  checkButtonPressed(buttonWindshield);
  checkButtonPressed(buttonFlash);
}
