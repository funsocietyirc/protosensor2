#include "configuration/setup.h"

Button button = Button(BUTTON_PIN, false, false, BUTTON_DEBOUNCE);

#ifdef SERIAL_DEBUG
Ticker serialDebugTicker(serialDebugTickerHandler, SERIAL_DEBUG_RATE);
#endif

#ifdef SERIAL_DEBUG
/**
 * Echo out MQTT messages over serial connection
 */
void serialDebugTickerHandler() {
  ++totalMqttMessages;
  snprintf(mqttMessage, 75, "hello world #%ld", totalMqttMessages);
  Serial.print("Publish message: ");
  Serial.println(mqttMessage);
}

#endif

/**
 * Handle Button Press
 */
void buttonHandler() {
  static unsigned long pressedMillis;
  static unsigned long ms;

  // Timing
  ms = millis();
  // Button
  button.read();

  // Deal with Button input
  if (button.wasPressed()) {
    // Record time pressed
    pressedMillis = ms;
    // Publish basic on message
    client.publish(BUTTON_BASIC_TOPIC, BUTTON_DOWN_PAYLOAD);
#ifdef BUTTON_BEEP_ON_PRESS
    // Turn on the beeper
    turnOn(BEEPER);
#endif
  } else if (button.wasReleased()) {
    // Record the time between start and stop presses
    unsigned int results = ms - pressedMillis;

    // convert to char array
    String resultsStr = String(results);
    unsigned int resultsStrLength = resultsStr.length();
    char output[resultsStrLength];
    resultsStr.toCharArray(output, resultsStrLength);

    // Send and reset the time pressed
    client.publish(BUTTON_PRESSED_TOPIC, output);
    pressedMillis = 0;
    // Send basic off message
    client.publish(BUTTON_BASIC_TOPIC, BUTTON_UP_PAYLOAD);
#ifdef BUTTON_BEEP_ON_PRESS
    // Turn off the beeper
    turnOff(BEEPER);
#endif
  }
}
