#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <CMMC_Config_Manager.h>
#include "FS.h"

CMMC_Config_Manager configManager;

void setup()
{
  Serial.begin(115200);
  Serial.println("begin");
  SPIFFS.begin();
  configManager.add_debug_listener([](const char* s) {
    // Serial.printf("-> %s\r\n", s);
  });

  configManager.init("/apconfig.json");
  configManager.add_field("maggie", "win!");
  configManager.commit();

  configManager.load_config([](JsonObject *root) {
    Serial.println("[user] json loaded..");
    root->printTo(Serial);
    Serial.println();
  });
}

void loop()
{
}
