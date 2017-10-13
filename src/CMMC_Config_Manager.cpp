#include "CMMC_Config_Manager.h"
#include "FS.h"

void CMMC_Config_Manager::setup() {
}

void CMMC_Config_Manager::load_config() {
  Serial.printf("FILENAME: %s \r\n", filename.c_str());
  File configFile = SPIFFS.open(filename.c_str(), "r");
  _user_debug_cb("Loading Config..");
  if (configFile) {
    size_t size = configFile.size();
      Serial.printf("config size = %lu\r\n", size);
    if (size > 512) {
      _user_debug_cb("Config file size is too large");
    } else {
      std::unique_ptr<char[]> buf(new char[size+1]);
      bzero(buf.get(), size+1);
      configFile.readBytes(buf.get(), size);
			strcpy(this->fileContent, buf.get());
			Serial.print("content: --->");
      Serial.print(buf.get());
			Serial.println("<---");

      this->parse_config();
		  configFile.close();
    }
  } else {
    _user_debug_cb("Failed to open config file");
		_write_json_file();
  }
}

void CMMC_Config_Manager::_write_json_file() {
    JsonObject& json = this->jsonBuffer.createObject();
		json["nat"] = 1;
    File configFile = SPIFFS.open(filename.c_str(), "w");
    if (!configFile) {
      _user_debug_cb("Failed to open config file for writing");
    }
    else {
      _user_debug_cb("REPLACE FILE OK!.");
      json.printTo(configFile);
			configFile.close();
    }
}

void CMMC_Config_Manager::parse_config() {
	Serial.println("Parsing confg...");
  JsonObject& json = this->jsonBuffer.parseObject(this->fileContent);
  if (json.success()) {
    this->_user_debug_cb("Parsing config success.");
    this->currentJsonObject = &json;
		Serial.print("content: --->");
    json.printTo(Serial);
		Serial.println("<---");
		dump_json_object();
  }
  else {
    _user_debug_cb("Failed to parse config file");
		_write_json_file();
  }
}

void CMMC_Config_Manager::dump_json_object() {
  JsonObject* obj = this->currentJsonObject;
  if (obj == NULL) {
		return;
	}
	else {
	  for (JsonObject::iterator it = obj->begin(); it != obj->end(); ++it) {
	    const char* key = it->key;
	    const char* value = it->value;
	    Serial.printf("%s:%s\r\n", key, value);
	    // _user_debug_cb()
	    // value = it->value.as<const char*>();
	  }
	}
}

void CMMC_Config_Manager::add_debug_listener(cmmc_debug_cb_t cb) {
  if (cb != NULL) {
    this->_user_debug_cb = cb;
  }
}
