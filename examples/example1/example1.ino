#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <CMMC_Config_Manager.h>
#include "FS.h"

CMMC_Config_Manager *configManager;

void setup()
{
  Serial.begin(115200);
  Serial.println("begin");
  configManager = new CMMC_Config_Manager("/config.json");
  configManager->add_debug_listener([](const char* s) {
    Serial.print("[USER]:");
    Serial.println(s);
  });

  configManager->setup();
  configManager->load_config();
}

void loop()
{

}
