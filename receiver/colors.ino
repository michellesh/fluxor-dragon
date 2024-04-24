CRGB getColorBetween(CRGB color1, CRGB color2, float percent) {
  return CRGB(
    color1.r + percent * (color2.r - color1.r),
    color1.g + percent * (color2.g - color1.g),
    color1.b + percent * (color2.b - color1.b)
  );
}

uint8_t getHueBetween(uint8_t hue1, uint8_t hue2, float percent) {
  return hue1 + percent * (float)(hue2 - hue1);
}

CRGB getGradientColorLeft(int strand, int pixel) {
  float percent = mapf(pixel, NUM_LEDS_WING[strand], 0, 0, 0.33333);
  //return CHSV(getHueBetween(hueLeft, hueRight, percent), 255, 255);
  return getColorBetween(colorLeft, colorRight, percent);
}

CRGB getGradientColorBelly(int strand, int pixel) {
  float percent = mapf(pixel, NUM_LEDS_BELLY[strand], 0, 0.33333, 0.66667);
  //return CHSV(getHueBetween(hueLeft, hueRight, percent), 255, 255);
  return getColorBetween(colorLeft, colorRight, percent);
}

CRGB getGradientColorRight(int strand, int pixel) {
  float percent = mapf(pixel, 0, NUM_LEDS_WING[strand], 0.66667, 1);
  //return CHSV(getHueBetween(hueLeft, hueRight, percent), 255, 255);
  return getColorBetween(colorLeft, colorRight, percent);
}

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
      ledsBelly[i][NUM_LEDS_BELLY[i] - j - 1] = color;
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
