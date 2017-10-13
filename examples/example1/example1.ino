#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <CMMC_Config_Manager.h>
#include "FS.h"

CMMC_Config_Manager *configManager;

void setup()
{
  Serial.begin(115200);
  Serial.println("begin");
  SPIFFS.begin();
  configManager = new CMMC_Config_Manager("/nat.json");
  configManager->add_debug_listener([](const char* s) {
    Serial.print("->");
    Serial.println(s);
  });

  configManager->init();
  configManager->load_config([](JsonObject *root) {
    if (root != NULL) {
      Serial.println("[user] json loaded..");
      root->printTo(Serial);
      Serial.println();
    }
  });
  configManager->save_config("nat", "ok!");
  configManager->save_config("mac", "1234567");
  // configManager->save_config("nickname", "99999999");
  // configManager->save_config("nickname2", "99999999");
  // configManager->load_config([](const JsonObject *root) {
  //   Serial.println("[user] json loaded..");
  //   root->printTo(Serial);
  // });
}

void loop()
{

}
