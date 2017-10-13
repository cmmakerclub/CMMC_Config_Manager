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
  USER_DEBUG_PRINTF("[save_config] with %s:%s", key.c_str(), value.c_str());
  load_config();
  USER_DEBUG_PRINTF("[[content]] %s", this->file_content);
  JsonObject& json = this->jsonBuffer.parseObject(this->file_content);
  json.set(key, value);
  json.printTo(Serial);
  configFile = SPIFFS.open(this->filename_c, "w");
  json.printTo(configFile);
  configFile.close();
  Serial.println();
  USER_DEBUG_PRINTF("/[save_config]");
}

void CMMC_Config_Manager::load_config() {
  USER_DEBUG_PRINTF("[load_config] Loading Config..");
  open_file();
  size_t size = configFile.size() + 1;
  std::unique_ptr<char[]> buf(new char[size + 1]);
  char *c = buf.get();
  bzero(buf.get(), size + 1);
  configFile.readBytes(c, size);
  configFile.close();
  strcpy(this->file_content, c);
  JsonObject& json = this->jsonBuffer.parseObject(buf.get());
  if (json.success()) {
    USER_DEBUG_PRINTF("[parse_config] Parsing config success.");
    json.printTo(Serial);
    Serial.println();
  }
  else {
    USER_DEBUG_PRINTF("[parse_config] Failed to parse config file.");
    _init_json_file();
  }
  USER_DEBUG_PRINTF("[load_config] config content ->%s<-", this->file_content);
  parse_config();
}

void CMMC_Config_Manager::_init_json_file() {
  USER_DEBUG_PRINTF("[_init_json_file]");
  configFile = SPIFFS.open(this->filename_c, "w");
  JsonObject& json = this->jsonBuffer.createObject();
  json.printTo(configFile);
  USER_DEBUG_PRINTF("[_init_json_file] closing file..");
  configFile.close();
}

void CMMC_Config_Manager::parse_config() {
}

void CMMC_Config_Manager::dump_json_object(cmmc_dump_cb_t printer) {
  USER_DEBUG_PRINTF("content = ->%s<-\r\n", this->file_content);
  this->load_config();
  USER_DEBUG_PRINTF("content = ->%s<-\r\n", this->file_content);
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
