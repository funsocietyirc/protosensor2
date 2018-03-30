#include "configuration/setup.h"

Button button = Button(BUTTON_PIN, false, false, BUTTON_DEBOUNCE);

#ifdef SERIAL_DEBUG
Ticker serialDebugTicker(serialDebugTickerHandler, SERIAL_DEBUG_RATE);
#endif

#ifdef SERIAL_DEBUG
void serialDebugTickerHandler() {
  ++totalMqttMessages;
  snprintf(mqttMessage, 75, "hello world #%ld", totalMqttMessages);
  Serial.print("Publish message: ");
  Serial.println(mqttMessage);
}

#endif

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
    client.publish("protosensor2/button", "on");
    // Turn on the beeper
    turnOn(BEEPER);
  } else if (button.wasReleased()) {
    // Record the time between start and stop presses
    unsigned int results = ms - pressedMillis;

    // convert to char array
    String resultsStr = String(results);
    unsigned int resultsStrLength = resultsStr.length();
    char output[resultsStrLength];
    resultsStr.toCharArray(output, resultsStrLength);

    // Send and reset the time pressed
    client.publish("protosensor2/pressed", output);
    pressedMillis = 0;
    // Send basic off message
    client.publish("protosensor2/button", "off");
    // Turn off the beeper
    turnOff(BEEPER);
  }
}
