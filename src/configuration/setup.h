#ifndef HEADER_CONFIG
#define HEADER_CONFIG

// Pin Outs
#include "SpireLib.h"
#include "Ticker.h" // https://github.com/sstaub/Ticker
#include <Button.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define BUTTON_PIN 4
#define DATA_PIN 12
#define LOAD_PIN 13
#define CLOCK_PIN 15
#define DIAL_PIN A0

// Comment out to use DHCP
#define USE_STATIC_IP

// Comment out to disable serial debugging
// #define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
#include <SPI.h>
#endif

// Button debounce
#define BUTTON_DEBOUNCE 5

#define SERIAL_DEBUG_RATE 10000
#define DIAL_RATE 1000

// PINS Available
enum pins { YELLOW1, RED1, GREEN1, BLUE1, RED2, GREEN2, WHITE1, BEEPER };

// Misc
extern byte shiftData;
extern byte previousShiftData;

// Objects
extern WiFiClient espClient;
extern PubSubClient client;
extern Button button;
extern Pot pwmPot;

// Functions

// WIfi
extern void setupWifi();

// Helpers
extern void initializePins();
extern void updateShiftData();
extern void turnOn(pins pin);
extern void turnOff(pins pin);
extern byte getValue(pins pin);
extern void updatePin(pins pin, char *value);
extern void clearPins();

// Handlers
#ifdef SERIAL_DEBUG
extern void serialDebugTickerHandler();
#endif
extern void buttonHandler();
extern void dialTickerHandler();

#ifdef SERIAL_DEBUG
extern Ticker serialDebugTicker;
#endif
extern Ticker dialTicker;

// MQTT
extern void mqttMessageHandler(char *topic, byte *payload, unsigned int length);
extern void mqttInit();
extern void reconnect();

#endif /* end of include guard: CONFIG */
