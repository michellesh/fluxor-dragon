/*
#define SPINE_LASER_LENGTH 10 // TODO 20

Scale spineLaserScaleSpeed = {1, 10, 0.1, 0.6, true}; // TODO 0.5, 3.0
Scale spineLaserFadeIn = {0, SPINE_LASER_LENGTH / 2, 0, 255, true};
Scale spineLaserFadeOut = {0, SPINE_LASER_LENGTH / 2, 255, 0, true};


int getOffsetPixelSpine(int pixel) {
  return pixel < NUM_LEDS_SPINE ? pixel : pixel - NUM_LEDS_SPINE;
}

void spineLaser() {
  if (spineLaserPixel < NUM_LEDS_SPINE) {
    int startPixel = (int)spineLaserPixel;
    int middlePixel = startPixel + (SPINE_LASER_LENGTH / 2);
    int endPixel = startPixel + SPINE_LASER_LENGTH;

    for (int pixel = 0; pixel < startPixel; pixel++) {
      int p = getOffsetPixelSpine(pixel);
      if (p < NUM_LEDS_SPINE) {
        //ledsSpine[p] = CRGB::Black;
      }
    }
    for (int pixel = startPixel; pixel < middlePixel; pixel++) {
      int b = spineLaserFadeIn.scale(pixel - startPixel);
      int p = getOffsetPixelSpine(pixel);
      if (p < NUM_LEDS_SPINE) {
        float percent = (float)p / (float)NUM_LEDS_SPINE;
        ledsSpine[p] = ledsSpine[p].nscale8(b);
      }
    }
    for (int pixel = middlePixel; pixel < endPixel; pixel++) {
      int b = spineLaserFadeOut.scale(pixel - middlePixel);
      int p = getOffsetPixelSpine(pixel);
      if (p < NUM_LEDS_SPINE) {
        float percent = (float)p / (float)NUM_LEDS_SPINE;
        ledsSpine[p] = ledsSpine[p].nscale8(b);
      }
    }
    for (int pixel = endPixel; pixel < NUM_LEDS_SPINE; pixel++) {
      int p = getOffsetPixelSpine(pixel);
      if (p < NUM_LEDS_SPINE) {
        //ledsSpine[p] = CRGB::Black;
      }
    }

    spineLaserPixel += spineLaserScaleSpeed.scale(speed);
  }
}
*/

void spinePride() {
  pride();
}

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

  for(uint16_t i = 0 ; i < NUM_LEDS_SPINE; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);


    float percent = (float)i / (float)NUM_LEDS_SPINE;
    CRGB color = getColorBetween(colorLeft, colorRight, percent);
    CRGB newcolor = color.nscale8(bri8 / 2);
    //CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS_SPINE - 1) - pixelnumber;

    nblend(ledsSpine[pixelnumber], newcolor, 64);
  }
}
