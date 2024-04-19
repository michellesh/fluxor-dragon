#define LASER_LENGTH 20

//auto laserScaleSpeed = scale(1, 10, 0.5, 3.0, true);
//auto laserFadeIn = scale(0, LASER_LENGTH / 2, 0, 255, true);
//auto laserFadeOut = scale(0, LASER_LENGTH / 2, 255, 0, true);

Scale laserScaleSpeed = {1, 10, 1.0, 3.0, true};

void lasers() {
  Serial.print("laserScaleSpeed(5) ");
  Serial.println(laserScaleSpeed.scale(5));
}
