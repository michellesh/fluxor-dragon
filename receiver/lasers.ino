#define LASER_LENGTH 10 // TODO 20

Scale laserScaleSpeed = {1, 10, 0.1, 0.6, true}; // TODO 0.5, 3.0
Scale laserFadeIn = {0, LASER_LENGTH / 2, 0, 255, true};
Scale laserFadeOut = {0, LASER_LENGTH / 2, 255, 0, true};

int getOffsetPixel(int pixel) {
  return pixel < MAX_LEDS_WING ? pixel : pixel - MAX_LEDS_WING;
}

void advanceLaserPixel() {
  laserPixel += laserScaleSpeed.scale(speed);
  if (laserPixel > MAX_LEDS_WING) {
    laserPixel -= MAX_LEDS_WING;
  }
}

void lasers() {
  int startPixel = (int)laserPixel;
  int middlePixel = startPixel + (LASER_LENGTH / 2);
  int endPixel = startPixel + LASER_LENGTH;

  for (int strand = 0; strand < NUM_STRIPS_WING; strand++) {
    for (int pixel = 0; pixel < startPixel; pixel++) {
      int p = getOffsetPixel(pixel);
      if (p < NUM_LEDS_WING[strand]) {
        ledsLeft[strand][p] = CRGB::Black;
        ledsRight[strand][p] = CRGB::Black;
      }
    }
    for (int pixel = startPixel; pixel < middlePixel; pixel++) {
      int b = laserFadeIn.scale(pixel - startPixel);
      int p = getOffsetPixel(pixel);
      if (p < NUM_LEDS_WING[strand]) {
        CRGB color = getGradientColorLeft(strand, pixel);
        ledsLeft[strand][p] = color.nscale8(b);
        color = getGradientColorRight(strand, pixel);
        ledsRight[strand][p] = color.nscale8(b);
      }
    }
    for (int pixel = middlePixel; pixel < endPixel; pixel++) {
      int b = laserFadeOut.scale(pixel - middlePixel);
      int p = getOffsetPixel(pixel);
      if (p < NUM_LEDS_WING[strand]) {
        CRGB color = getGradientColorLeft(strand, pixel);
        ledsLeft[strand][p] = color.nscale8(b);
        color = getGradientColorRight(strand, pixel);
        ledsRight[strand][p] = color.nscale8(b);
      }
    }
    for (int pixel = endPixel; pixel < MAX_LEDS_WING; pixel++) {
      int p = getOffsetPixel(pixel);
      if (p < NUM_LEDS_WING[strand]) {
        ledsLeft[strand][p] = CRGB::Black;
        ledsRight[strand][p] = CRGB::Black;
      }
    }
  }

  advanceLaserPixel();
}
