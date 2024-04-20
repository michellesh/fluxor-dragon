Scale knobToHue = {1000, 0, 255, 0, true};
Scale knobToSpeed = {1000, 0, 1, 11, true};

bool knobValueChanged(Knob knob) {
  int BUFFER = 20;
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
  int value = knob.pin == KNOB_COLOR_LEFT ? colorLeft.value
    : knob.pin == KNOB_COLOR_RIGHT ? colorRight.value
    : speed.value;

  Serial.print(knob.description);
  Serial.print(" changed: ");
  Serial.println(value);
}

void readKnobValue(Knob &knob) {
  knob.value = analogRead(knob.pin);
}
