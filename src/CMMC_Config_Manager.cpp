#include "CMMC_Config_Manager.h"
#include "FS.h"

void CMMC_Config_Manager::init() {
    USER_DEBUG_PRINTF("initializing...");
    SPIFFS.begin();
    Dir dir = SPIFFS.openDir("/");
     while (dir.next()) {
       String fileName = dir.fileName();
       size_t fileSize = dir.fileSize();
       USER_DEBUG_PRINTF("FS File: %s, size: %s\n", fileName.c_str(), String(fileSize).c_str());
     }
    open_file();
}

void CMMC_Config_Manager::load_config() {
  USER_DEBUG_PRINTF("Loading Config..");
  if (_status == 1) {
    size_t size = configFile.size() + 1;
    std::unique_ptr<char[]> buf(new char[size + 1]);
    this->file_content_ptr = buf.get();
    bzero(this->file_content_ptr, size + 1);
    configFile.readBytes(this->file_content_ptr, size);
    USER_DEBUG_PRINTF("config content ->%s<-", this->file_content_ptr);
    this->parse_config();
  } else {
    USER_DEBUG_PRINTF("load_config skipped due to invalid config file.");
  }
}

void CMMC_Config_Manager::_init_json_file() {
  USER_DEBUG_PRINTF("[_init_json]");
  configFile = SPIFFS.open(this->filename_c, "w");
  JsonObject& json = this->jsonBuffer.createObject();
  json.printTo(Serial);
  json.printTo(configFile);
  USER_DEBUG_PRINTF("closing file..");
  configFile.close();
}

void CMMC_Config_Manager::parse_config() {
  JsonObject& json = this->jsonBuffer.parseObject(this->file_content_ptr);
  if (json.success()) {
    USER_DEBUG_PRINTF("Parsing config success.");
    this->currentJsonObject = &json;
  }
  else {
    USER_DEBUG_PRINTF("Failed to parse config file");
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
