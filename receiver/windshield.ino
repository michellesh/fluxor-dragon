#define ANGLE_THRESHOLD 10 // how wide the area of LEDs lit up is while spinning

Scale spinSpeed = {1, 10, 0.3, 10, true}; // TODO 1, 20

void initPixelAngles() {
  for (int strand = 0; strand < NUM_STRIPS_WING; strand++) {
    for (int pixel = 0; pixel < NUM_LEDS_WING[strand]; pixel++) {
      int x = xLeft[strand][pixel];
      int y = yLeft[strand][pixel];
      pixelAnglesLeft[strand][pixel] = getPixelAngle(x, y);
    }
  }
  for (int strand = 0; strand < NUM_STRIPS_WING; strand++) {
    for (int pixel = 0; pixel < NUM_LEDS_WING[strand]; pixel++) {
      int x = xRight[strand][pixel];
      int y = yRight[strand][pixel];
      pixelAnglesRight[strand][pixel] = getPixelAngle(x, y);
    }
  }
}

int getPixelAngle(int x, int y) {
  int d = degrees(atan2(y - centerY, x - centerX)) + 90;
  return d < 0 ? 360 + d : d;
}

uint8_t angleDiffToBrightness(int diff) {
  return map(diff, 0, ANGLE_THRESHOLD, 255, 0);
}

void setPixelWindshield(int strand, int pixel) {
  int diffLeft = abs(spinAngle - pixelAnglesLeft[strand][pixel]);
  if (diffLeft < ANGLE_THRESHOLD) {
    CRGB color = getGradientColorLeft(strand, pixel);
    ledsLeft[strand][pixel] = color.nscale8(angleDiffToBrightness(diffLeft));
  }

  int diffRight = abs(spinAngle - pixelAnglesRight[strand][pixel]);
  if (diffRight < ANGLE_THRESHOLD) {
    CRGB color = getGradientColorRight(strand, pixel);
    ledsRight[strand][pixel] = color.nscale8(angleDiffToBrightness(diffRight));
  }
}

void windshield() {
  for (int strand = 0; strand < NUM_STRIPS_WING; strand++) {
    fadeToBlackBy(ledsLeft[strand], NUM_LEDS_WING[strand], 20);
    fadeToBlackBy(ledsRight[strand], NUM_LEDS_WING[strand], 20);
  }

  for (int strand = 0; strand < NUM_STRIPS_WING; strand++) {
    for (int pixel = 0; pixel < NUM_LEDS_WING[strand]; pixel++) {
      setPixelWindshield(strand, pixel);
    }
  }

  int angleMax = 135; // the angle at which the windshield wiper bounces on the
                      // right side and starts going backwards
  int angleMin = 225; // the angle at which the windshield wiper bounces on the
                      // left side and starts going forward
  spinAngle += (spinReverse ? -1 : 1) * spinSpeed.scale(speed);
  if (spinReverse) { // backward
    if (spinAngle < 0) {
      spinAngle += 360;
    }
    if (spinAngle < angleMin && spinAngle > angleMax) {
      spinReverse = false;
    }
  } else { // forward
    if (spinAngle > 360) {
      spinAngle -= 360;
    }
    if (spinAngle > angleMax && spinAngle < angleMin) {
      spinReverse = true;
    }
  }
}

void setPixelFlap(int strand, int pixel) {
  int diffLeft = abs((360 - spinAngle) - pixelAnglesLeft[strand][pixel]);
  if (diffLeft < ANGLE_THRESHOLD) {
    CRGB color = getGradientColorLeft(strand, pixel);
    ledsLeft[strand][pixel] = color.nscale8(angleDiffToBrightness(diffLeft));
  }

  int diffRight = abs(spinAngle - pixelAnglesRight[strand][pixel]);
  if (diffRight < ANGLE_THRESHOLD) {
    CRGB color = getGradientColorRight(strand, pixel);
    ledsRight[strand][pixel] = color.nscale8(angleDiffToBrightness(diffRight));
  }
}

void flap() {
  for (int strand = 0; strand < NUM_STRIPS_WING; strand++) {
    fadeToBlackBy(ledsLeft[strand], NUM_LEDS_WING[strand], 20);
    fadeToBlackBy(ledsRight[strand], NUM_LEDS_WING[strand], 20);
  }

  for (int strand = 0; strand < NUM_STRIPS_WING; strand++) {
    for (int pixel = 0; pixel < NUM_LEDS_WING[strand]; pixel++) {
      setPixelFlap(strand, pixel);
    }
  }

  // Update angle
  int angleMax = 135; // the angle at which the windshield wiper bounces on the
                      // right side and starts going backwards
  int angleMin = 0; // the angle at which the windshield wiper bounces on the
                      // left side and starts going forward
  spinAngle += (spinReverse ? -1 : 1) * spinSpeed.scale(speed);
  if (spinReverse && spinAngle < angleMin) {
    spinReverse = false;
  } else if (!spinReverse && spinAngle > angleMax) {
    spinReverse = true;
  }
}
