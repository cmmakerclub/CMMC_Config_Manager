#include "CMMC_Config_Manager.h"
#include "FS.h"

void CMMC_Config_Manager::setup() {
}

void CMMC_Config_Manager::load_config() {
  _user_debug_cb("Loading Config..");
  if (_status == 1) {
    size_t size = configFile.size() + 1;
    std::unique_ptr<char[]> buf(new char[size + 1]);
    this->file_content_ptr = buf.get();
    bzero(this->file_content_ptr, size + 1);
    configFile.readBytes(this->file_content_ptr, size);
    this->parse_config();
  } else {
    _user_debug_cb("Failed to open config file");
    _init_json_file();
  }
}

void CMMC_Config_Manager::_init_json_file() {
  JsonObject& json = this->jsonBuffer.createObject();
  json["nat"] = 1;
  json.printTo(configFile);
}

void CMMC_Config_Manager::parse_config() {
  JsonObject& json = this->jsonBuffer.parseObject(this->file_content_ptr);
  if (json.success()) {
    this->_user_debug_cb("Parsing config success.");
    this->currentJsonObject = &json;
  }
  else {
    _user_debug_cb("Failed to parse config file");
    _init_json_file();
  }
}

void CMMC_Config_Manager::dump_json_object(cmmc_dump_cb_t printer) {
  // if (printer == NULL) printer = _user_debug_cb;
  // printer("dumping json object");
  if (this->currentJsonObject == NULL) {
    return;
  }
  else {
    char str_buffer[30];
    JsonObject* obj = this->currentJsonObject;
    for (JsonObject::iterator it = obj->begin(); it != obj->end(); ++it) {
      const char* key = it->key;
      const char* value = it->value;
      sprintf(str_buffer, "[key] %s -> %s\r\n", key, value);
      printer(str_buffer, key, value);
    }
  }
}

void CMMC_Config_Manager::add_debug_listener(cmmc_debug_cb_t cb) {
  if (cb != NULL) {
    this->_user_debug_cb = cb;
  }
}
