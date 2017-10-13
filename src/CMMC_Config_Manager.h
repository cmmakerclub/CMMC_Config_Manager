#ifndef CMMC_Config_Manager_H
#define CMMC_Config_Manager_H

#include "ESP8266WiFi.h"
#include <functional>
#include <ArduinoJson.h>
#include "FS.h"

#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif


typedef void (*cmmc_err_status_t)(u8 status, const char* cause);
typedef void (*cmmc_succ_status_t)(u8 status);
typedef void (*cmmc_debug_cb_t)(const char* cause);
typedef void (*cmmc_dump_cb_t)(const char* msg, const char* k, const char* v);

class CMMC_Config_Manager
{
    public:
        // constructure
        CMMC_Config_Manager(const char* filename) {
          strcpy(this->filename_c, filename);
          this->_user_debug_cb = [](const char* s) { };
          open_file();
        }

        ~CMMC_Config_Manager() {}
        void setup();
        void load_config();
        void add_debug_listener(cmmc_debug_cb_t cb);
        void parse_config();
        void dump_json_object(cmmc_dump_cb_t printer);
        void open_file() {
          SPIFFS.begin();
          Serial.printf("filename: %s\r\n", this->filename_c);
          configFile = SPIFFS.open(this->filename_c, "r+");
          if (configFile) {
            size_t size = configFile.size();
            Serial.printf("config size = %lu\r\n", size);
            if (size > 512) {
              Serial.printf("Config file size is too large");
            } else {
              _status = 1;
            }
          }
          else {
            Serial.printf("openfile failed.\r\n");
          }
        }
    private:
        void _init_json_file();
        StaticJsonBuffer<300> jsonBuffer;
        JsonObject* currentJsonObject = NULL;
        cmmc_debug_cb_t _user_debug_cb;

        char filename_c[60];
        char *file_content_ptr;
        File configFile;

        u8 _status = 0;

};

#endif //CMMC_Config_Manager_H
