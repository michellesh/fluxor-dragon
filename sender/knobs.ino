#define POTENTIOMETER_MAX 4095

Scale knobToHue = {POTENTIOMETER_MAX, 0, 255, 0, true};
Scale knobToSpeed = {POTENTIOMETER_MAX, 0, 1, 11, true};

bool knobValueChanged(Knob knob) {
  int BUFFER = 100;
  return knob.value < (knob.prev - BUFFER) || knob.value > (knob.prev + BUFFER);
}

void checkKnobChanged(Knob &knob) {
  readKnobValue(knob);
  if (knobValueChanged(knob)) {
    onKnobChanged(knob);
    printKnobText(knob);
  }
}

void onKnobChanged(Knob &knob) {
  switch (knob.pin) {
    case KNOB_COLOR_EYE:
      colorEye.value = knobToHue.scale(knobColorEye.value);
      send(colorEye);
      break;
    case KNOB_COLOR_LEFT:
      colorLeft.value = knobToHue.scale(knobColorLeft.value);
      send(colorLeft);
      break;
    case KNOB_COLOR_RIGHT:
      colorRight.value = knobToHue.scale(knobColorRight.value);
      send(colorRight);
      break;
    case KNOB_SPEED:
      speed.value = knobToSpeed.scale(knobSpeed.value);
      send(speed);
      break;
    default: break;
  }
  knob.prev = knob.value;
}

void printKnobText(Knob &knob) {
  int value = knob.pin == KNOB_COLOR_EYE ? colorEye.value
    : knob.pin == KNOB_COLOR_LEFT ? colorLeft.value
    : knob.pin == KNOB_COLOR_RIGHT ? colorRight.value
    : speed.value;

  Serial.print(knob.description);
  Serial.print(" changed: ");
  Serial.println(value);
}

void readKnobValue(Knob &knob) {
  knob.value = analogRead(knob.pin);
}

/*
void sendKnobValues() {
  readKnobValue(knobColorEye);
  readKnobValue(knobColorLeft);
  readKnobValue(knobColorRight);
  readKnobValue(knobSpeed);
  onKnobChanged(knobColorLeft);
  delay(500);
  onKnobChanged(knobColorRight);
  delay(500);
  onKnobChanged(knobColorEye);
  delay(500);
  onKnobChanged(knobSpeed);
  delay(500);
  printKnobText(knobColorEye);
  printKnobText(knobColorLeft);
  printKnobText(knobColorRight);
  printKnobText(knobSpeed);
}
*/
