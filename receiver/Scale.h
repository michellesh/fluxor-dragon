/* Example usage:
 *
 *  ```
 *  // Set up a scale to convert values from 1-10 to 1.0-3.0
 *  Scale speedScale = {1, 10, 1.0, 3.0, true};
 *
 *  float result = speedScale.scale(5); // Convert 5 to a value 1.0-3.0
 *  Serial.println(result); // Output should be 1.89
 *
 *  ```
 */

struct Scale {
  float domainStart;
  float domainEnd;
  float rangeStart;
  float rangeEnd;
  bool clamp;
  float scale(float value) {
    float percentage = (value - domainStart) / (domainEnd - domainStart);
    float newValue = rangeStart + (rangeEnd - rangeStart) * percentage;
    return clamp && newValue < min(rangeStart, rangeEnd)
               ? min(rangeStart, rangeEnd)
           : clamp && newValue > max(rangeStart, rangeEnd)
               ? max(rangeStart, rangeEnd)
               : newValue;
  }
};
