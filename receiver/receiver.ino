#include <WiFi.h>
#include <FastLED.h>
#include <esp_now.h>
#include "fluxor-dragon-shared.h"

#include "Scale.h"

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
#define MAX_LEDS_WING NUM_LEDS_WING_1

#define NUM_LEDS_BELLY_1 25
#define NUM_LEDS_BELLY_2 25
#define NUM_LEDS_BELLY_3 25
#define NUM_LEDS_BELLY_4 25
#define NUM_LEDS_BELLY_5 25
#define NUM_LEDS_BELLY_6 25
#define MAX_LEDS_BELLY NUM_LEDS_BELLY_1

#define NUM_LEDS_EYES 9
#define NUM_LEDS_SPINE 50

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

CRGB ledsRight1[NUM_LEDS_WING_1];
CRGB ledsRight2[NUM_LEDS_WING_2];
CRGB ledsRight3[NUM_LEDS_WING_3];
CRGB ledsRight4[NUM_LEDS_WING_4];
CRGB ledsRight5[NUM_LEDS_WING_5];
CRGB ledsRight6[NUM_LEDS_WING_6];
CRGB *ledsRight[] = {ledsRight1, ledsRight2, ledsRight3,
                     ledsRight4, ledsRight5, ledsRight6};

CRGB ledsBelly1[NUM_LEDS_BELLY_1];
CRGB ledsBelly2[NUM_LEDS_BELLY_2];
CRGB ledsBelly3[NUM_LEDS_BELLY_3];
CRGB ledsBelly4[NUM_LEDS_BELLY_4];
CRGB ledsBelly5[NUM_LEDS_BELLY_5];
CRGB ledsBelly6[NUM_LEDS_BELLY_6];
CRGB *ledsBelly[] = {ledsBelly1, ledsBelly2, ledsBelly3,
                     ledsBelly4, ledsBelly5, ledsBelly6};

CRGB ledsEyes[NUM_LEDS_EYES];
CRGB ledsSpine[NUM_LEDS_SPINE];

byte boardNumber;

void setup() {
  Serial.begin(9600);
  delay(500);

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
}

void loop() {
  EVERY_N_SECONDS(1) {
    Serial.print("boardNumber: ");
    Serial.println(boardNumber);
    lasers();
  }

  CRGB colors[] = {CRGB::Red,   CRGB::Orange, CRGB::Yellow,
                   CRGB::Green, CRGB::Blue,   CRGB::Purple};

  // Wings
  for (int i = 0; i < NUM_STRIPS_WING; i++) {
    for (int j = 0; j < NUM_LEDS_WING[i]; j++) {
      ledsLeft[i][j] = colors[NUM_STRIPS_WING - 1 - i];
    }
  }
  for (int i = 0; i < NUM_STRIPS_WING; i++) {
    for (int j = 0; j < NUM_LEDS_WING[i]; j++) {
      ledsRight[i][j] = colors[i];
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

  // Belly
  for (int i = 0; i < NUM_STRIPS_BELLY; i++) {
    for (int j = 0; j < NUM_LEDS_BELLY[i]; j++) {
      ledsBelly[i][j] = colors[NUM_STRIPS_BELLY - 1 - i];
    }
  }

  FastLED.show();
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
