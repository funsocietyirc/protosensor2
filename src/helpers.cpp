#include "configuration/setup.h"

byte shiftData = B00000000;
byte previousShiftData = 0;

/**
 * Setup Pin modes
 */
void initializePins() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LOAD_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  digitalWrite(LOAD_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, shiftData);
  digitalWrite(LOAD_PIN, HIGH);
}

void turnAllLedsOff() {}

/**
 * Update Shift register
 */
void updateShiftData() {
  if (previousShiftData != shiftData) {
    digitalWrite(LOAD_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, shiftData);
    digitalWrite(LOAD_PIN, HIGH);
    previousShiftData = shiftData;
  }
}

/**
 * Clear Pins
 */
void clearPins() { shiftData = 0; }

/**
 * Update a pin
 * @param pin   Pin to update
 * @param value Value to update it to
 */
void updatePin(pins pin, char *value) { bitWrite(shiftData, pin, value); }

/**
 * Turn On a PIN
 * @param pin PIN Assignment
 */
void turnOn(pins pin) {
  bitSet(shiftData, pin);
  updateShiftData();
}

/**
 * Turn Off a PIN
 * @param pins PIN assignment
 */
void turnOff(pins pin) {
  bitClear(shiftData, pin);
  updateShiftData();
}

/**
 * Get a pins value
 * @param pins PIN assignment
 */
byte getValue(pins pin) { return bitRead(shiftData, pin); }
