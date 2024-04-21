#define ANGLE_THRESHOLD 10 // how wide the area of LEDs lit up is while spinning

Scale spinSpeed = {1, 10, 0.3, 10, true}; // TODO 1, 20

/*
int xBellyScaleToLength(int strand, int xBelly) {
  return map(xBelly, xMinBelly, xMaxBelly, xMinBelly, xMinBelly + NUM_LEDS_BELLY[strand]);
}
*/

void initPixelAngles() {
  for (int strand = 0; strand < NUM_STRIPS_WING; strand++) {
    for (int pixel = 0; pixel < NUM_LEDS_WING[strand]; pixel++) {
      int x = xLeft[strand][pixel];
      int y = yLeft[strand][pixel];
      //int cx = xLeft[strand][0] + 1;
      //int cy = yLeft[strand][0] + 1;
      pixelAnglesLeft[strand][pixel] = getPixelAngle(x, y);
    }
  }
  for (int strand = 0; strand < NUM_STRIPS_WING; strand++) {
    for (int pixel = 0; pixel < NUM_LEDS_WING[strand]; pixel++) {
      int x = xRight[strand][pixel];
      int y = yRight[strand][pixel];
      //int cx = xRight[strand][0] - 1;
      //int cy = yRight[strand][0] - 1;
      pixelAnglesRight[strand][pixel] = getPixelAngle(x, y);
    }
  }
  /*
  // Belly. Calculate both pixelAnglesBelly and xBelly
  for (int strand = 0; strand < NUM_STRIPS_BELLY; strand++) {
    for (int pixel = 0; pixel < NUM_LEDS_BELLY[strand]; pixel++) {
      float xDist = (float)pixel * 1.3333333; // LEDs are 1.3333" apart
      int xPosition = (int)((float)xBellyStart - xDist); // Belly LEDs run backwards
      xBelly[strand][pixel] = xBellyScaleToLength(strand, xPosition);
      int x = xBelly[strand][pixel];
      int y = yBelly[strand];
      pixelAnglesBelly[strand][pixel] = getPixelAngle(x, y);
    }
  }
  */
}

int getPixelAngle(int x, int y) {
  // int c = xyMax / 2; // both cx and cy (circle origin)
  // int d = degrees(atan2(y - cy, x - cx)) + 90;
  int d = degrees(atan2(y - centerY, x - centerX)) + 90;
  return d < 0 ? 360 + d : d;
}

uint8_t angleDiffToBrightness(int diff) {
  return map(diff, 0, ANGLE_THRESHOLD, 255, 0);
}

void setStrandInThreshold(int strand, int pixel) {
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
      setStrandInThreshold(strand, pixel);
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

void setWingStrandInThresholdFlap(int strand, int pixel) {
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

/*
void setBellyStrandInThresholdFlap(int strand, int pixel) {
  int diff = abs(spinAngle - pixelAnglesBelly[strand][pixel]);
  if (diff < ANGLE_THRESHOLD) {
    CRGB color = getGradientColorBelly(strand, pixel);
    ledsBelly[strand][pixel] = color.nscale8(angleDiffToBrightness(diff));
  }
}
*/

void flappingWings() {
  for (int strand = 0; strand < NUM_STRIPS_WING; strand++) {
    fadeToBlackBy(ledsLeft[strand], NUM_LEDS_WING[strand], 20);
    fadeToBlackBy(ledsRight[strand], NUM_LEDS_WING[strand], 20);
  }
  //for (int strand = 0; strand < NUM_STRIPS_BELLY; strand++) {
  //  fadeToBlackBy(ledsBelly[strand], NUM_LEDS_BELLY[strand], 20);
  //}

  for (int strand = 0; strand < NUM_STRIPS_WING; strand++) {
    for (int pixel = 0; pixel < NUM_LEDS_WING[strand]; pixel++) {
      setWingStrandInThresholdFlap(strand, pixel);
    }
  }
  //for (int strand = 0; strand < NUM_STRIPS_BELLY; strand++) {
  //  for (int pixel = 0; pixel < NUM_LEDS_BELLY[strand]; pixel++) {
  //    setBellyStrandInThresholdFlap(strand, pixel);
  //  }
  //}

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
