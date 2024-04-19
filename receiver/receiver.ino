#include <FastLED.h>

uint8_t receiverAddress1[] = {0xA8, 0x42, 0xE3, 0xAB, 0x80, 0xD4};
uint8_t receiverAddress2[] = {0xA0, 0xA3, 0xB3, 0x2D, 0x4E, 0x58};
uint8_t senderAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
char receiverMacAddress1[] = "A8:42:E3:AB:80:D4";
char receiverMacAddress2[] = "A0:A3:B3:2D:4E:58";
char senderMacAddress[] = "";

#define LED_PIN  19
#define NUM_LEDS 25
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

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2813, LED_PIN, GRB>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
}

void loop() {
  pride();
  FastLED.show();
}

// This function is taken from the FastLED example Pride2015
void pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for(uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
}
