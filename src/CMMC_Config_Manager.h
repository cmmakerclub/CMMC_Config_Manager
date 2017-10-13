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
typedef void (*cmmc_json_loaded_cb_t)(JsonObject* root);

#define USER_DEBUG_PRINTF(fmt, args...) { sprintf(this->debug_buffer, fmt, ## args); _user_debug_cb(this->debug_buffer); }

class CMMC_Config_Manager
{
  public:
    // constructure
    CMMC_Config_Manager(const char* filename) {
      strcpy(this->filename_c, filename);
      this->_user_debug_cb = [](const char* s) { };
    }

    ~CMMC_Config_Manager() {
      configFile.close();
    }

    void init();
    void load_config(cmmc_json_loaded_cb_t cb = NULL);
    void add_debug_listener(cmmc_debug_cb_t cb);
    // void parse_config();
    void save_config(String key, String value);
    void dump_json_object(cmmc_dump_cb_t printer);
    void open_file() {
      USER_DEBUG_PRINTF("[open_file] open filename: %s", this->filename_c);
      if (SPIFFS.exists(this->filename_c)) {
        configFile = SPIFFS.open(this->filename_c, "r");
        USER_DEBUG_PRINTF("[open_file] config size = %lu bytes", configFile.size());
        if (configFile.size() > 512) {
          USER_DEBUG_PRINTF("[open_file] Config file size is too large");
        } else {
          USER_DEBUG_PRINTF("[open_file] check file size ok.");
        }
      } else { // file not exists
        USER_DEBUG_PRINTF("[open_file] file not existsing so create a new file");
        _init_json_file();
      }
    }
  private:
    void _init_json_file(cmmc_json_loaded_cb_t cb = NULL);
    StaticJsonBuffer<300> jsonBuffer;
    cmmc_debug_cb_t _user_debug_cb;
    File configFile;
    char filename_c[60];
    char debug_buffer[60];
    u8 _status = 0;
    String _k;
    String _v;
};

#endif //CMMC_Config_Manager_H
