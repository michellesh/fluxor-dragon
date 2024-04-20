CRGB getColorBetween(CRGB color1, CRGB color2, float percent) {
  return CRGB(
    color1.r + percent * (color2.r - color1.r),
    color1.g + percent * (color2.g - color1.g),
    color1.b + percent * (color2.b - color1.b)
  );
}

CRGB getGradientColorLeft(int strand, int pixel) {
  float xAdjusted = (float)xLeft[strand][pixel] - (float)xMin;
  float xTotalWidth = (float)xMax - (float)xMin;
  float percent = xAdjusted / xTotalWidth;
  return getColorBetween(colorLeft, colorRight, percent);
}

CRGB getGradientColorRight(int strand, int pixel) {
  float xAdjusted = (float)xRight[strand][pixel] - (float)xMin;
  float xTotalWidth = (float)xMax - (float)xMin;
  float percent = xAdjusted / xTotalWidth;
  return getColorBetween(colorLeft, colorRight, percent);
}

/*
CRGB getStrandModeColor(int strand, int pixel) {
  if (colorMode == COLOR_MODE_WHEEL) {
    return getStrandWheelColor(strand, pixel);
  } else if (colorMode == COLOR_MODE_GRADIENT) {
    return getStrandGradientColor(strand, pixel);
  } else if (colorMode == COLOR_MODE_SOLID) {
    return activeColor;
  } else {
    return CRGB(0, 0, 0);
  }
}
*/

CRGB knobValueToColor(int knobValue) {
  float ywCutoff = 3;  // Percent Yellow/White Cutoff Threshold
  float knobPercent = mapf((float)knobValue, 0, 255, 0, 100);
  if (knobPercent < ywCutoff) {
    float percent = mapf((float)knobPercent, 0, ywCutoff, 0, 100);
    return getColorBetween(CRGB::White, CRGB::Yellow, percent / 100);
  } else {
    float hue = mapf(knobPercent, ywCutoff, 100, (float)HUE_YELLOW, 255);
    return CHSV(hue, 255, 255);
  }
}

void setAllColor(CRGB color) {
  // Wings
  for (int i = 0; i < NUM_STRIPS_WING; i++) {
    for (int j = 0; j < NUM_LEDS_WING[i]; j++) {
      ledsLeft[i][j] = color;
    }
  }
  for (int i = 0; i < NUM_STRIPS_WING; i++) {
    for (int j = 0; j < NUM_LEDS_WING[i]; j++) {
      ledsRight[i][j] = color;
    }
  }

  // Belly
  for (int i = 0; i < NUM_STRIPS_BELLY; i++) {
    for (int j = 0; j < NUM_LEDS_BELLY[i]; j++) {
      ledsBelly[i][j] = color;
    }
  }

  // Eyes
  for (int i = 0; i < NUM_LEDS_EYES; i++) {
    ledsEyes[i] = color;
  }

  // Spine
  for (int i = 0; i < NUM_LEDS_SPINE; i++) {
    ledsSpine[i] = color;
  }
}
