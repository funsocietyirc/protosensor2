#include "configuration/credentials.h"
#include "configuration/setup.h"

#ifdef USE_STATIC_IP
IPAddress ip(192, 168, 1, 119);
IPAddress dns(192, 168, 1, 2);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 1, 1);
#endif

WiFiClient espClient;

/**
 * Setup Wifi
 */
void setupWifi() {
  delay(10);
#ifdef SERIAL_DEBUG
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
#endif

#ifdef USE_STATIC_IP
  WiFi.config(ip, dns, subnet, gateway);
#endif
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
#ifdef SERIAL_DEBUG
    Serial.println("Connection Failed! Rebooting...");
#endif
  }

  randomSeed(micros());

#ifdef SERIAL_DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif
}
