#include "CMMC_Config_Manager.h"
#include "FS.h"

void CMMC_Config_Manager::init() {
    USER_DEBUG_PRINTF("initializing...");
    SPIFFS.begin();
    Dir dir = SPIFFS.openDir("/");
     while (dir.next()) {
       String fileName = dir.fileName();
       size_t fileSize = dir.fileSize();
       USER_DEBUG_PRINTF("FS File: %s, size: %s", fileName.c_str(), String(fileSize).c_str());
     }
}

void CMMC_Config_Manager::save_config(String key, String value) {
  USER_DEBUG_PRINTF(">>>[save_config] with %s:%s", key.c_str(), value.c_str());
  static CMMC_Config_Manager *_this = this;
  this->_k = key;
  this->_v = value;
  load_config([](JsonObject *root) {
      _this->configFile = SPIFFS.open(_this->filename_c, "w");
      Serial.printf("[saveeeeeeeeee] json loaded..%s:%s",
        _this->_k.c_str(), _this->_v.c_str());
      root->set(_this->_k, _this->_v);
      root->printTo(Serial);
      root->printTo(_this->configFile);
      Serial.println();
      _this->configFile.close();
  });
  USER_DEBUG_PRINTF(">>>/[save_config]");
}

void CMMC_Config_Manager::load_config(cmmc_json_loaded_cb_t cb) {
  USER_DEBUG_PRINTF("[load_config] Loading Config..");
  open_file();
  size_t size = configFile.size() + 1;
  std::unique_ptr<char[]> buf(new char[size + 1]);
  bzero(buf.get(), size + 1);
  configFile.readBytes(buf.get(), size);
  configFile.close();
  USER_DEBUG_PRINTF("[load_config] config content ->%s<-", buf.get());
  JsonObject& json = this->jsonBuffer.parseObject(buf.get());
  if (json.success()) {
    USER_DEBUG_PRINTF("[load_config] Parsing config success.");
    if (cb != NULL) {
    USER_DEBUG_PRINTF("[load_config] calling callback fn");
      cb(&json);
    }
  }
  else {
    USER_DEBUG_PRINTF("[load_config] Failed to parse config file.");
    _init_json_file(cb);
  }
}

void CMMC_Config_Manager::_init_json_file(cmmc_json_loaded_cb_t cb) {
  USER_DEBUG_PRINTF("[_init_json_file]");
  configFile = SPIFFS.open(this->filename_c, "w");
  JsonObject& json = this->jsonBuffer.createObject();
  json.printTo(configFile);
  USER_DEBUG_PRINTF("[_init_json_file] closing file..");
  configFile.close();
  load_config(cb);
}

void CMMC_Config_Manager::dump_json_object(cmmc_dump_cb_t printer) {
  this->load_config();
  // if (this->currentJsonObject == NULL) {
  //   return;
  // }
  // else {
  //   this->currentJsonObject->printTo(Serial);
  //   char str_buffer[30];
  //   JsonObject* obj = this->currentJsonObject;
  //   for (JsonObject::iterator it = obj->begin(); it != obj->end(); ++it) {
  //     const char* key = it->key;
  //     const char* value = it->value;
  //     sprintf(str_buffer, "[key] %s -> %s\r\n", key, value);
  //     printer(str_buffer, key, value);
  //   }
  // }
}

void CMMC_Config_Manager::add_debug_listener(cmmc_debug_cb_t cb) {
  if (cb != NULL) {
    this->_user_debug_cb = cb;
  }
}
