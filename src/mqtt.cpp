#include "configuration/credentials.h"
#include "configuration/setup.h"

PubSubClient client(espClient);

#ifdef SERIAL_DEBUG
unsigned int totalMqttMessages = 0;
#endif

/**
 * Handle MQTT Callback
 * @param topic   MQTT Topic
 * @param payload MQTT Pay
 * @param length  MQTT Payload length
 */
void mqttMessageHandler(char *topic, byte *payload, unsigned int length) {
#ifdef SERIAL_DEBUG
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
#endif

  // Convert Byte pointer to string
  payload[length] = '\0';
  String out = String((char *)payload);

#ifdef SERIAL_DEBUG
  Serial.println(out);
  Serial.println();
#endif

  // Handle Topics
  if (strcmp(topic, "protosensor2/beep") == 0) {
    turnOn(BEEPER);
    int delaytime = out.toInt();
    delay(delaytime > 500 ? 500 : delaytime);
    turnOff(BEEPER);
  }
  // Remap in the future, remain compliant with desk leds 1.0
  else if (strcmp(topic, "office/desk_leds/output/yellow_1/set") == 0) {
    if (out.equals("ON")) {
      turnOn(YELLOW1);
      client.publish("office/desk_leds/output/yellow_1", "ON");
    } else if (out.equals("OFF")) {
      turnOff(YELLOW1);
      client.publish("office/desk_leds/output/yellow_1", "OFF");
    }
  } else if (strcmp(topic, "office/desk_leds/output/white_1/set") == 0) {
    if (out.equals("ON")) {
      turnOn(WHITE1);
      client.publish("office/desk_leds/output/white_1", "ON");
    } else if (out.equals("OFF")) {
      turnOff(WHITE1);
      client.publish("office/desk_leds/output/white_1", "OFF");
    }
  } else if (strcmp(topic, "office/desk_leds/output/red_1/set") == 0) {
    if (out.equals("ON")) {
      turnOn(RED1);
      client.publish("office/desk_leds/output/red_1", "ON");
    } else if (out.equals("OFF")) {
      turnOff(RED1);
      client.publish("office/desk_leds/output/red_1", "OFF");
    }
  } else if (strcmp(topic, "office/desk_leds/output/red_2/set") == 0) {
    if (out.equals("ON")) {
      turnOn(RED2);
      client.publish("office/desk_leds/output/red_2", "ON");
    } else if (out.equals("OFF")) {
      turnOff(RED2);
      client.publish("office/desk_leds/output/red_2", "OFF");
    }
  } else if (strcmp(topic, "office/desk_leds/output/green_1/set") == 0) {
    if (out.equals("ON")) {
      turnOn(GREEN1);
      client.publish("office/desk_leds/output/green_1", "ON");
    } else if (out.equals("OFF")) {
      turnOff(GREEN1);
      client.publish("office/desk_leds/output/green_1", "OFF");
    }
  } else if (strcmp(topic, "office/desk_leds/output/green_2/set") == 0) {
    if (out.equals("ON")) {
      turnOn(GREEN2);
      client.publish("office/desk_leds/output/green_2", "ON");
    } else if (out.equals("OFF")) {
      turnOff(GREEN2);
      client.publish("office/desk_leds/output/green_2", "OFF");
    }
  }
  // NEED TO REMAP
  else if (strcmp(topic, "office/desk_leds/output/round/set") == 0) {
    if (out.equals("ON")) {
      turnOn(BLUE1);
      client.publish("office/desk_leds/output/round", "ON");
    } else if (out.equals("OFF")) {
      turnOff(BLUE1);
      client.publish("office/desk_leds/output/round", "OFF");
    }
  }
}

void mqttInit() {
  client.publish("protosensor2/status", "online");
  client.subscribe("protosensor2/#");
  client.subscribe("office/desk_leds/#");
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

    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str(),   // Client ID
                       MQTT_USER,          // User Name
                       MQTT_PASSWORD,      // Password
                       "protosensor2/lwt", // WillTopic
                       1, true, "offline")) {
#ifdef SERIAL_DEBUG
      Serial.println("connected");
#endif
      mqttInit();
    } else {
#ifdef SERIAL_DEBUG
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
#endif

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
