/* Example usage: 
 * 
 *  ```
 *  Timer myTimer = {10000}; // Set up a timer to last 10 seconds (10000 milliseconds)
 *  
 *  myTimer.reset(); // Start the timer
 *  
 *  if (myTimer.complete()) {
 *  
 *    // ... Do something here ...
 *  
 *    myTimer.reset(); // Start the timer again (if you want)
 *  }
 *  ```
 */

struct Timer {
  unsigned long totalCycleTime;
  unsigned long lastCycleTime;
  void reset() { lastCycleTime = millis(); };
  bool complete() { return (millis() - lastCycleTime) > totalCycleTime; };
};
