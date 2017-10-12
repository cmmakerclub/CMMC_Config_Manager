#ifndef CMMC_Config_Manager_H
#define CMMC_Config_Manager_H

#include "ESP8266WiFi.h"
#include <functional>
#include <ArduinoJson.h>

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
        CMMC_Config_Manager(String f) {
          this->_user_debug_cb = [](const char* s) { };
          strcpy(this->filename_c, filename.c_str());
          this->filename = String(this->filename_c);
        }

        CMMC_Config_Manager(const char* f) {
          this->filename = String(f);
        }

        ~CMMC_Config_Manager() {}
        void setup();
        void load_config();
        void add_debug_listener(cmmc_debug_cb_t cb);
        void parse_config();
    private:
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject* currentJsonObject = NULL;
      cmmc_debug_cb_t _user_debug_cb;

      char filename_c[60];
      String filename;
      char* file_content_ptr;

};

#endif //CMMC_Config_Manager_H
