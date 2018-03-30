#include "configuration/credentials.h"
#include "configuration/setup.h"

PubSubClient client(espClient);

const char *onPayload = "ON";
const char *offPayload = "OFF";

#ifdef SERIAL_DEBUG
unsigned int totalMqttMessages = 0;
#endif

/**
 * Handle a generic pin toggle
 * @param pin        The pin to toggle
 * @param root       The namespace to report to
 * @param payloadStr The payload String
 */
void handleToggle(pins pin, char *root, String payloadStr) {
  if (payloadStr.equals(onPayload)) {
    turnOn(pin);
    client.publish(root, onPayload);
  } else if (payloadStr.equals(offPayload)) {
    turnOff(pin);
    client.publish(root, offPayload);
  }
}

/**
 * Parse and handle topics
 * @param topicStr   The topic
 * @param payloadStr The payload
 */
void handleTopic(String topicStr, String payloadStr) {
  // Handle Topics
  if (topicStr.equals(BEEPER_TOPIC)) {
    turnOn(BEEPER);
    int delaytime = payloadStr.toInt();
    // TODO get rid of this delay
    delay(delaytime > BEEPER_MAX_DELAY ? BEEPER_MAX_DELAY : delaytime);
    turnOff(BEEPER);
  }
  // Remap in the future, remain compliant with desk leds 1.0
  else if (topicStr.equals("office/desk_leds/output/yellow_1/set")) {
    handleToggle(YELLOW1, (char *)"office/desk_leds/output/yellow_1",
                 payloadStr);
  } else if (topicStr.equals("office/desk_leds/output/white_1/set")) {
    handleToggle(WHITE1, (char *)"office/desk_leds/output/white_1", payloadStr);
  } else if (topicStr.equals("office/desk_leds/output/red_1/set")) {
    handleToggle(RED1, (char *)"office/desk_leds/output/red_1", payloadStr);
  } else if (topicStr.equals("office/desk_leds/output/red_2/set")) {
    handleToggle(RED2, (char *)"office/desk_leds/output/red_2", payloadStr);
  } else if (topicStr.equals("office/desk_leds/output/green_1/set")) {
    handleToggle(GREEN1, (char *)"office/desk_leds/output/green_1", payloadStr);
  } else if (topicStr.equals("office/desk_leds/output/green_2/set")) {
    handleToggle(GREEN2, (char *)"office/desk_leds/output/green_2", payloadStr);
  } else if (topicStr.equals("office/desk_leds/output/round/set")) {
    handleToggle(BLUE1, (char *)"office/desk_leds/output/round", payloadStr);
  }
}

/**
 * Handle MQTT Callback
 * @param topic   MQTT Topic
 * @param payload MQTT Pay
 * @param length  MQTT Payload length
 */
void mqttMessageHandler(char *topic, byte *payload, unsigned int length) {
  // Convert Byte pointer to string
  payload[length] = '\0';
  String payloadStr = String((char *)payload);

  // Convert Topic string
  String topicStr = String(topic);

  // Parse and handle topics
  handleTopic(topicStr, payloadStr);

#ifdef SERIAL_DEBUG
  Serial.print("Message arrived:");
  Serial.println("Topic: " + topicStr);
  Serial.println("Payload: " + payloadStr);
  Serial.println();
#endif
}

/**
 * Publish LWT alive message and subscribe to channels
 */
void mqttInit() {
  client.publish(MQTT_LWT_TOPIC, MQTT_LWT_ANOUNCE);
  // Prevent Initial Beep caused by stored message
  client.publish(BEEPER_TOPIC, "0");
  client.subscribe(ROOT_TOPIC);
  client.subscribe(LEDS_TOPIC);
}

/**
 * Reconnect to network services
 */
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
#ifdef SERIAL_DEBUG
    Serial.print("Attempting MQTT connection...");
#endif

#ifdef MQTT_CLIENT_ID
    String clientId = MQTT_CLIENT_ID;
#else
    // Create a random client ID
    String clientId = "Protosensor-";
    clientId += String(random(0xffff), HEX);
#endif

    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD,
                       MQTT_LWT_TOPIC, MQTT_TOS, MQTT_PERSIST,
                       MQTT_LWT_PAYLOAD)) {
#ifdef SERIAL_DEBUG
      Serial.println("connected");
#endif
      mqttInit();
    } else {
#ifdef SERIAL_DEBUG
      Serial.println("failed, rc= " + client.state());
#endif

      // Wait 5 seconds before retrying
      delay(MQTT_RECONNECT_DELAY);
    }
  }
}
