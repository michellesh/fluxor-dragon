bool isButtonPressed(Button button) {
  return !digitalRead(button.pin);
}

void checkButtonPressed(Button &button) {
  if (isButtonPressed(button)) {
    if (!button.pressed) {
      button.pressed = true;
      printButtonText(button);
      onButtonPressed(button);
    }
  } else if (button.pressed) {
    button.pressed = false;
    onButtonUnpressed(button);
  }
}

void onButtonPressed(Button &button) {
  switch (button.pin) {
    case BUTTON_LASERS:
      background.value = VIZ_LASERS;
      send(background);
      //backgroundTimer.reset();
      break;
    case BUTTON_TWINKLE:
      background.value = VIZ_TWINKLE;
      send(background);
      //backgroundTimer.reset();
      break;
    case BUTTON_WINDSHIELD:
      background.value = VIZ_WINDSHIELD;
      send(background);
      //backgroundTimer.reset();
      break;
    case BUTTON_FLASH:
      send(strobeOff); // Big button has opposite values
      break;
    default: break;
  }
}

void onButtonUnpressed(Button &button) {
  switch (button.pin) {
    case BUTTON_FLASH:
      send(strobeOn); // Big button has opposite values
      break;
    default: break;
  }
}

void printButtonText(Button &button) {
  Serial.print("Button pressed: ");
  Serial.println(button.description);
}
