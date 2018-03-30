#include "configuration/setup.h"

unsigned int dial = 0;
unsigned int newDial = 0;

Pot pwmPot(DIAL_PIN);

Ticker dialTicker(dialTickerHandler, DIAL_RATE);

void dialTickerHandler() {
  newDial = pwmPot.readPWM();
  // Dial has changed since last cycle
  if (dial != newDial && abs(dial - newDial) > 5) {
    int newDialScaled = ((float)newDial / 255.0) * 100.0;
    String str = String(newDialScaled);
    int str_len = str.length() + 1;
    char char_array[str_len];
    str.toCharArray(char_array, str_len);
    client.publish("protosensor2/brightness", char_array);
    dial = newDial;
  }
}
