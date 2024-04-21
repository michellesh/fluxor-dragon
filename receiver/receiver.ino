// clang-format off
#include <FastLED.h>
#include <WiFi.h>
#include <esp_now.h>

#include "utils.h"

#include "fluxor-dragon-shared.h"

#include "Scale.h"
#include "Timer.h"
// clang-format on

#define BRIGHTNESS 255

// RECEIVER BOARD 1
#define PIN_EYES 15
#define PIN_SPINE 13
#define PIN_LEFT_1 27
#define PIN_LEFT_2 33
#define PIN_LEFT_3 26
#define PIN_LEFT_4 32
#define PIN_LEFT_5 14
#define PIN_LEFT_6 12
#define PIN_RIGHT_1 4
#define PIN_RIGHT_2 2
#define PIN_RIGHT_3 21
#define PIN_RIGHT_4 19
#define PIN_RIGHT_5 23
#define PIN_RIGHT_6 22

// RECEIVER BOARD 2
#define PIN_BELLY_1 19
#define PIN_BELLY_2 21
#define PIN_BELLY_3 13
#define PIN_BELLY_4 14
#define PIN_BELLY_5 27
#define PIN_BELLY_6 26

#define NUM_STRIPS_WING 6
#define NUM_STRIPS_BELLY 6

#define NUM_LEDS_WING_1 25
#define NUM_LEDS_WING_2 25
#define NUM_LEDS_WING_3 25
#define NUM_LEDS_WING_4 25
#define NUM_LEDS_WING_5 25
#define NUM_LEDS_WING_6 25
#define MAX_LEDS_WING NUM_LEDS_WING_1 // TODO
#define MIN_LEDS_WING NUM_LEDS_WING_6 // TODO

#define NUM_LEDS_BELLY_1 25
#define NUM_LEDS_BELLY_2 25
#define NUM_LEDS_BELLY_3 25
#define NUM_LEDS_BELLY_4 25
#define NUM_LEDS_BELLY_5 25
#define NUM_LEDS_BELLY_6 25
#define MAX_LEDS_BELLY NUM_LEDS_BELLY_1 // TODO
#define MIN_LEDS_BELLY NUM_LEDS_BELLY_6 // TODO

#define NUM_LEDS_EYES 12
#define NUM_LEDS_SPINE 50

#include "xy.h"

int NUM_LEDS_WING[] = {NUM_LEDS_WING_1, NUM_LEDS_WING_2, NUM_LEDS_WING_3,
                       NUM_LEDS_WING_4, NUM_LEDS_WING_5, NUM_LEDS_WING_6};
int NUM_LEDS_BELLY[] = {NUM_LEDS_BELLY_1, NUM_LEDS_BELLY_2, NUM_LEDS_BELLY_3,
                        NUM_LEDS_BELLY_4, NUM_LEDS_BELLY_5, NUM_LEDS_BELLY_6};

CRGB ledsLeft1[NUM_LEDS_WING_1];
CRGB ledsLeft2[NUM_LEDS_WING_2];
CRGB ledsLeft3[NUM_LEDS_WING_3];
CRGB ledsLeft4[NUM_LEDS_WING_4];
CRGB ledsLeft5[NUM_LEDS_WING_5];
CRGB ledsLeft6[NUM_LEDS_WING_6];
CRGB *ledsLeft[] = {ledsLeft1, ledsLeft2, ledsLeft3,
                    ledsLeft4, ledsLeft5, ledsLeft6};

int pixelAnglesLeft1[NUM_LEDS_WING_1];
int pixelAnglesLeft2[NUM_LEDS_WING_2];
int pixelAnglesLeft3[NUM_LEDS_WING_3];
int pixelAnglesLeft4[NUM_LEDS_WING_4];
int pixelAnglesLeft5[NUM_LEDS_WING_5];
int pixelAnglesLeft6[NUM_LEDS_WING_6];
int *pixelAnglesLeft[] = {pixelAnglesLeft1, pixelAnglesLeft2, pixelAnglesLeft3,
                          pixelAnglesLeft4, pixelAnglesLeft5, pixelAnglesLeft6};

CRGB ledsRight1[NUM_LEDS_WING_1];
CRGB ledsRight2[NUM_LEDS_WING_2];
CRGB ledsRight3[NUM_LEDS_WING_3];
CRGB ledsRight4[NUM_LEDS_WING_4];
CRGB ledsRight5[NUM_LEDS_WING_5];
CRGB ledsRight6[NUM_LEDS_WING_6];
CRGB *ledsRight[] = {ledsRight1, ledsRight2, ledsRight3,
                     ledsRight4, ledsRight5, ledsRight6};

int pixelAnglesRight1[NUM_LEDS_WING_1];
int pixelAnglesRight2[NUM_LEDS_WING_2];
int pixelAnglesRight3[NUM_LEDS_WING_3];
int pixelAnglesRight4[NUM_LEDS_WING_4];
int pixelAnglesRight5[NUM_LEDS_WING_5];
int pixelAnglesRight6[NUM_LEDS_WING_6];
int *pixelAnglesRight[] = {pixelAnglesRight1, pixelAnglesRight2, pixelAnglesRight3,
                           pixelAnglesRight4, pixelAnglesRight5, pixelAnglesRight6};

CRGB ledsBelly1[NUM_LEDS_BELLY_1];
CRGB ledsBelly2[NUM_LEDS_BELLY_2];
CRGB ledsBelly3[NUM_LEDS_BELLY_3];
CRGB ledsBelly4[NUM_LEDS_BELLY_4];
CRGB ledsBelly5[NUM_LEDS_BELLY_5];
CRGB ledsBelly6[NUM_LEDS_BELLY_6];
CRGB *ledsBelly[] = {ledsBelly1, ledsBelly2, ledsBelly3,
                     ledsBelly4, ledsBelly5, ledsBelly6};

int pixelAnglesBelly1[NUM_LEDS_BELLY_1];
int pixelAnglesBelly2[NUM_LEDS_BELLY_2];
int pixelAnglesBelly3[NUM_LEDS_BELLY_3];
int pixelAnglesBelly4[NUM_LEDS_BELLY_4];
int pixelAnglesBelly5[NUM_LEDS_BELLY_5];
int pixelAnglesBelly6[NUM_LEDS_BELLY_6];
int *pixelAnglesBelly[] = {pixelAnglesBelly1, pixelAnglesBelly2, pixelAnglesBelly3,
                           pixelAnglesBelly4, pixelAnglesBelly5, pixelAnglesBelly6};

CRGB ledsEyes[NUM_LEDS_EYES];
CRGB ledsSpine[NUM_LEDS_SPINE];

byte boardNumber;
float laserPixel = 0;
float spineLaserPixel = NUM_LEDS_SPINE;
float spinAngle = 0;
bool spinReverse = false;
uint8_t speed = DEFAULT_SPEED;
CRGB colorLeft = DEFAULT_COLOR_LEFT;
CRGB colorRight = DEFAULT_COLOR_RIGHT;
CRGB colorEye = DEFAULT_COLOR_EYE;
uint8_t activeViz = DEFAULT_VIZ;
bool strobeOn = false;

int xMax = 0; // calculated in setup()
int xMin = 999;

Timer laserTimer = {500, 0}; // 1000 = 1 second

CRGB rainbow[] = {CRGB::Red,   CRGB::Orange, CRGB::Yellow,
                  CRGB::Green, CRGB::Blue,   CRGB::Purple};

msg data;

void printActionInfo(msg data, uint8_t len) {
  switch (data.action) {
    case ACTION_SET_COLOR_LEFT:
      Serial.print("ACTION_SET_COLOR_LEFT: ");
      Serial.println(data.value);
      break;
    case ACTION_SET_COLOR_RIGHT:
      Serial.print("ACTION_SET_COLOR_RIGHT: ");
      Serial.println(data.value);
      break;
    case ACTION_SPEED:
      Serial.print("ACTION_SPEED: ");
      Serial.println(data.value);
      break;
    case ACTION_SET_BACKGROUND:
      Serial.print("ACTION_SET_BACKGROUND: ");
      Serial.println(data.value);
      break;
    case ACTION_STROBE_ON:
      Serial.println("ACTION_STROBE_ON");
      break;
    case ACTION_STROBE_OFF:
      Serial.println("ACTION_STROBE_OFF");
      break;
    default:
      Serial.print("Bytes received: ");
      Serial.println(len);
      Serial.print("action: ");
      Serial.println(data.action);
      Serial.print("value: ");
      Serial.println(data.value);
      Serial.println();
      break;
  }
}

//callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));

  printActionInfo(data, len);

  // KNOB ACTIONS
  if (data.action == ACTION_SET_COLOR_LEFT) {
    CRGB color = knobValueToColor(data.value);
    colorLeft = color;
  } else if (data.action == ACTION_SET_COLOR_RIGHT) {
    CRGB color = knobValueToColor(data.value);
    colorRight = color;
  } else if (data.action == ACTION_SET_COLOR_EYE) {
    CRGB color = knobValueToColor(data.value);
    colorEye = color;
  } else if (data.action == ACTION_SPEED) {
    speed = data.value;

  // BUTTON ACTIONS
  } else if (data.action == ACTION_SET_BACKGROUND) {
    activeViz = data.value;
    if (activeViz == VIZ_WINDSHIELD) {
      spinAngle = 0;
    } else if (activeViz == VIZ_LASERS) {
      laserPixel = 0;
    }
  } else if (data.action == ACTION_STROBE_ON) {
    strobeOn = true;
    spineLaserPixel = 0;
  } else if (data.action == ACTION_STROBE_OFF) {
    strobeOn = false;
  }
}

void setup() {
  Serial.begin(9600);
  delay(500);

  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  boardNumber = getBoardNumber();

  if (boardNumber == 1) {
    // Left wing
    FastLED.addLeds<WS2813, PIN_LEFT_1, GRB>(ledsLeft[0], NUM_LEDS_WING_1)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_LEFT_2, GRB>(ledsLeft[1], NUM_LEDS_WING_2)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_LEFT_3, GRB>(ledsLeft[2], NUM_LEDS_WING_3)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_LEFT_4, GRB>(ledsLeft[3], NUM_LEDS_WING_4)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_LEFT_5, GRB>(ledsLeft[4], NUM_LEDS_WING_5)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_LEFT_6, GRB>(ledsLeft[5], NUM_LEDS_WING_6)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);

    // Right wing
    FastLED.addLeds<WS2813, PIN_RIGHT_1, GRB>(ledsRight[0], NUM_LEDS_WING_1)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_RIGHT_2, GRB>(ledsRight[1], NUM_LEDS_WING_2)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_RIGHT_3, GRB>(ledsRight[2], NUM_LEDS_WING_3)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_RIGHT_4, GRB>(ledsRight[3], NUM_LEDS_WING_4)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_RIGHT_5, GRB>(ledsRight[4], NUM_LEDS_WING_5)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_RIGHT_6, GRB>(ledsRight[5], NUM_LEDS_WING_6)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);

    // Eyes and Spine
    FastLED.addLeds<WS2813, PIN_EYES, GRB>(ledsEyes, NUM_LEDS_EYES)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_SPINE, GRB>(ledsSpine, NUM_LEDS_SPINE)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);

  } else if (boardNumber == 2) {
    // Belly
    FastLED.addLeds<WS2813, PIN_BELLY_1, GRB>(ledsBelly[0], NUM_LEDS_BELLY_1)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_BELLY_2, GRB>(ledsBelly[1], NUM_LEDS_BELLY_2)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_BELLY_3, GRB>(ledsBelly[2], NUM_LEDS_BELLY_3)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_BELLY_4, GRB>(ledsBelly[3], NUM_LEDS_BELLY_4)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_BELLY_5, GRB>(ledsBelly[4], NUM_LEDS_BELLY_5)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<WS2813, PIN_BELLY_6, GRB>(ledsBelly[5], NUM_LEDS_BELLY_6)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
  }

  // Set min and max X values
  for (int i = 0; i < NUM_STRIPS_WING; i++) {
    for (int j = 0; j < NUM_LEDS_WING[i]; j++) {
      if (xRight[i][j] > xMax) {
        xMax = xRight[i][j];
      }
      if (xLeft[i][j] < xMin) {
        xMin = xLeft[i][j];
      }
    }
  }

  initPixelAngles();
}

void loop() {
  EVERY_N_SECONDS(1) {
    //Serial.print("boardNumber: ");
    //Serial.println(boardNumber);
  }

  EVERY_N_MILLISECONDS(20) {
    if (activeViz == VIZ_TWINKLE) {
      twinkle();
    } else if (activeViz == VIZ_LASERS) {
      lasers();
      showBellyNoAnimation();
    } else if (activeViz == VIZ_WINDSHIELD) {
      //windshield();
      flap();
      showBellyNoAnimation();
    } else {
      setAllColor(CRGB(0, 0, 0));
    }
  }

  // Eyes
  for (int i = 0; i < NUM_LEDS_EYES; i++) {
    ledsEyes[i] = colorEye;
    if (!strobeOn) {
      ledsEyes[i].nscale8(BRIGHTNESS / 3);
    }

  }

  // Spine
  spinePride();
  //spineLaser();
  if (strobeOn) {
    for (int i = 0; i < NUM_LEDS_SPINE; i++) {
      float percent = (float)i / (float)NUM_LEDS_SPINE;
      ledsSpine[i] = getColorBetween(colorLeft, colorRight, percent);
    }
  }

  FastLED.show();
}

void showBellyNoAnimation() {
  for (int i = 0; i < NUM_STRIPS_BELLY; i++) {
    for (int j = 0; j < NUM_LEDS_BELLY[i]; j++) {
      CRGB color = getGradientColorBelly(i, j);
      ledsBelly[i][j] = color.nscale8(BRIGHTNESS / 2);
    }
  }
}

void testLEDs() {
  // Wings
  for (int i = 0; i < NUM_STRIPS_WING; i++) {
    for (int j = 0; j < NUM_LEDS_WING[i]; j++) {
      ledsLeft[i][j] = rainbow[NUM_STRIPS_WING - 1 - i];
    }
  }
  for (int i = 0; i < NUM_STRIPS_WING; i++) {
    for (int j = 0; j < NUM_LEDS_WING[i]; j++) {
      ledsRight[i][j] = rainbow[i];
    }
  }

  // Belly
  for (int i = 0; i < NUM_STRIPS_BELLY; i++) {
    for (int j = 0; j < NUM_LEDS_BELLY[i]; j++) {
      ledsBelly[i][j] = rainbow[NUM_STRIPS_BELLY - 1 - i];
    }
  }

  // Eyes
  for (int i = 0; i < NUM_LEDS_EYES; i++) {
    ledsEyes[i] = CRGB::White;
  }

  // Spine
  for (int i = 0; i < NUM_LEDS_SPINE; i++) {
    ledsSpine[i] = CRGB::Pink;
  }
}

// Reads the MAC address of this ESP32 and assigns a number 1 or 2
byte getBoardNumber() {
  byte boardNumber = WiFi.macAddress() == receiverMacAddress1   ? 1
                     : WiFi.macAddress() == receiverMacAddress2 ? 2
                                                                : 0;
  if (boardNumber == 0) {
    Serial.print("MAC address not found: ");
    Serial.println(WiFi.macAddress());
  } else {
    Serial.print("Board number: ");
    Serial.println(boardNumber);
  }

  return boardNumber;
}
