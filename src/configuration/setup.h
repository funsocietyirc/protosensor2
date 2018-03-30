#ifndef HEADER_CONFIG
#define HEADER_CONFIG

// PINS Available
enum pins { YELLOW1, RED1, GREEN1, BLUE1, RED2, GREEN2, WHITE1, BEEPER };

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

// #define SERIAL_DEBUG // Comment out to disable serial debugging

#ifdef SERIAL_DEBUG
#include <SPI.h>
#define SERIAL_DEBUG_RATE 10000
#define SERIAL_DEBUG_BAUD 115200
#endif

// Button debounce
#define BUTTON_BEEP_ON_PRESS // Comment to stop beep
#define BUTTON_DEBOUNCE 5
#define BUTTON_DOWN_PAYLOAD "on"
#define BUTTON_UP_PAYLOAD "off"

#define STARTUP_DELAY 10
#define MQTT_RECONNECT_DELAY 5000
#define WIFI_CONNECT_DELAY 5000
#define BEEPER_MAX_DELAY 500

#define POT_RATE 1000
#define POT_THRESHOLD 5

#define MQTT_CLIENT_ID "protosensor2" // Comment out for random
#define MQTT_LWT_PAYLOAD "offline"
#define MQTT_LWT_ANOUNCE "online"
#define MQTT_TOS 1
#define MQTT_PERSIST true

// MQTT TOPICs
#define ROOT "protosensor2/"

// Subs
#define ROOT_TOPIC ROOT "#"
#define LEDS_TOPIC "office/desk_leds/#"
// Pubs
#define POT_TOPIC ROOT "brightness"
#define BUTTON_PRESSED_TOPIC ROOT "pressed"
#define BUTTON_BASIC_TOPIC ROOT "button"
#define MQTT_LWT_TOPIC ROOT "lwt"
#define BEEPER_TOPIC ROOT "beeper"

// Objects
extern WiFiClient espClient;
extern PubSubClient client;
extern Button button;
extern Pot pwmPot;

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
