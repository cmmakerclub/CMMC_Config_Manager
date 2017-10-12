#ifndef CMMC_Config_Manager_H
#define CMMC_Config_Manager_H

#include "ESP8266WiFi.h"
#include <functional>

#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

enum CMMC_Config_Manager_mode_t {
    MODE_AP, MODE_STA
};


class CMMC_Config_Manager
{
public:
    CMMC_Config_Manager_mode_t mode;

    // constructure
    CMMC_Config_Manager() {}
    
    ~CMMC_Config_Manager() {}
    
    void setup();

};

#endif //CMMC_Config_Manager_H
