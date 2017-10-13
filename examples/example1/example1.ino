#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <CMMC_Config_Manager.h>
#include "FS.h"

CMMC_Config_Manager *configManager;

void setup()
{
  Serial.begin(115200);
  Serial.println("begin");
  configManager = new CMMC_Config_Manager("/nat999.json");
  configManager->add_debug_listener([](const char* s) {
    Serial.print("[USER]:");
    Serial.println(s);
  });

  configManager->init();
  configManager->load_config();
  // configManager->dump_json_object([](const char* s,const char* k,
  //   const char* v) {
  //   Serial.printf("[USER]: %s->%s\r\n", k, v);
  // });
}

void loop()
{

}
