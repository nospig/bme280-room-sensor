#ifndef _weatherstation_h
#define _weatherstation_h

#include "DisplayBase.h"

// task callbacks
void connectWifiCallback();
void readSensorsCallback();
void updateThingSpeakCallback();
void updateWifiStrengthCallback();
void checkSettingsChangedCallback();
void mqttPublishCallback();

void mqttSubscribeCallback(const char* topic, const char *payload);

void setupOtaUpdates();

#endif // _weatherstation_h