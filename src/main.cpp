#include "configuration/credentials.h"
#include "configuration/setup.h"

/**
 * Initial Device Setup
 */
void setup() {
// Setup Serial
#ifdef SERIAL_DEBUG
  Serial.begin(SERIAL_DEBUG_BAUD);
#endif

  // Setup Network
  setupWifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqttMessageHandler);

  // Subscribe to topic
  if (client.connect(MQTT_SERVER, MQTT_USER, MQTT_PASSWORD)) {
    mqttInit();
  }

  // Initialize Shift Register
  initializePins();

// Initialize Tickers
#ifdef SERIAL_DEBUG
  serialDebugTicker.start();
#endif
  dialTicker.start();
}

/**
 * System Loop
 */
void loop() {
  // Update The Shifts
  updateShiftData();

  // Network
  if (!client.connected()) {
    reconnect();
  }

  // Handle MQTT Client
  client.loop();

// Tickers
#ifdef SERIAL_DEBUG
  serialDebugTicker.update();
#endif
  dialTicker.update();

  // Button
  buttonHandler();
}
