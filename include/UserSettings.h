#ifndef _user_settings_h
#define _user_settings_h

#include <Arduino.h>

// OTA password
#define OTA_MD5_PASSWORD_HASH "5f4dcc3b5aa765d61d8327deb882cf99"

// BME280 pins
// TFT pins changed in the library
const int BME_SDA = D2;
const int BME_SCL = D1;
const uint8_t BME_ADDRESS = 0x76;


#endif // _user_settings_h