#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <CMMC_Config_Manager.h>
#include "FS.h"

CMMC_Config_Manager instance("/config.json");
CMMC_Config_Manager instance2("/config2.json");

void setup()
{
  Serial.begin(115200);
  Serial.println("begin");
  Serial.println("begin");
  Serial.println(SPIFFS.begin());
  instance.add_debug_listener([](const char* s) {
    Serial.print("[USER]:");
    Serial.println(s);
  });

  instance2.add_debug_listener([](const char* s) {
    Serial.print("[USER]:");
    Serial.println(s);
  });

  instance.setup();
  instance2.setup();
}

void loop()
{

}
