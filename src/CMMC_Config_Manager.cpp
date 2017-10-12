#include "CMMC_Config_Manager.h"
#include "FS.h"

void CMMC_Config_Manager::setup() {
  String result = String(SPIFFS.begin());
  _user_debug_cb(result.c_str());
  load_config();
}

void CMMC_Config_Manager::load_config() {
  Serial.printf("FILENAME: %s \r\n", filename.c_str());
  File configFile = SPIFFS.open(filename.c_str(), "r");
  _user_debug_cb("Loading Config..");
  if (configFile) {
    size_t size = configFile.size();
    if (size > 1024) {
      _user_debug_cb("Config file size is too large");
    } else {
      std::unique_ptr<char[]> buf(new char[size]);
      this->file_content_ptr = buf.get();
      // load content
      configFile.readBytes(buf.get(), size);
      Serial.println(this->file_content_ptr);
      this->parse_config();
    }
  } else {
    _user_debug_cb("Failed to open config file");
  }
}

void CMMC_Config_Manager::parse_config() {
  JsonObject& json = this->jsonBuffer.parseObject(this->file_content_ptr);
  if (!json.success()) {
    this->currentJsonObject = &json;
    _user_debug_cb("Failed to parse config file");
  }
  else {
    this->_user_debug_cb("Parsing config success.");
  }
}

void CMMC_Config_Manager::add_debug_listener(cmmc_debug_cb_t cb) {
  if (cb != NULL) {
    this->_user_debug_cb = cb;
  }
}
