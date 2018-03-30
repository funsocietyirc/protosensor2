#include "configuration/setup.h"

unsigned int dial = 0;
unsigned int newDial = 0;

Pot pwmPot(DIAL_PIN);

Ticker dialTicker(dialTickerHandler, POT_RATE);

/**
 * Attempt to normalize the pot
 * @param  reading from pot
 * @return      an int from 0 to 100
 */
int normalizedPot(float dial) { return ((float)dial / 255.0) * 100.0; }

/**
 * Handle the dial ticker
 */
void dialTickerHandler() {
  newDial = pwmPot.readPWM();
  // Dial has changed since last cycle, and is in a acceptable threadhold
  if (dial != newDial && abs(dial - newDial) > POT_THRESHOLD) {
    int newDialScaled = normalizedPot(newDial);

    String str = String(newDialScaled);
    int str_len = str.length() + 1;
    char char_array[str_len];
    str.toCharArray(char_array, str_len);
    client.publish(POT_TOPIC, char_array);
    dial = newDial;
  }
}
