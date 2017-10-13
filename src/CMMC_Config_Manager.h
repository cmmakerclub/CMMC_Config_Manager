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

class CMMC_Config_Manager
{
    public:
        // constructure
        CMMC_Config_Manager(const char* filename) {
          strcpy(this->filename_c, filename);

          this->filename = String(this->filename_c);
          this->_user_debug_cb = [](const char* s) { };

          SPIFFS.begin();
        }

        ~CMMC_Config_Manager() {}
        void setup();
        void load_config();
        void add_debug_listener(cmmc_debug_cb_t cb);
        void parse_config();
        void dump_json_object();
    private:
        void _write_json_file();
        StaticJsonBuffer<300> jsonBuffer;
        JsonObject* currentJsonObject = NULL;
        cmmc_debug_cb_t _user_debug_cb;

        char filename_c[60];
        char fileContent[512];
        String filename;
};

#endif //CMMC_Config_Manager_H
