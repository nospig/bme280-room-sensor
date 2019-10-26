#ifndef _WebServer_H_
#define _WebServer_H_

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "SettingsManager.h"

class WebServer
{
    public:
        void init(SettingsManager* settingsManager);
        static AsyncWebServer* getServer();

        void updateSensorReadings(float temp, float humidity, float pressure);

        bool screenGrabRequested();
        void clearScreenGrabRequest();

    private:
        static void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
        static void updateClientOnConnect();
        static String tokenProcessor(const String& token);
        static void handleUpdateThingSpeakSettings(AsyncWebServerRequest* request);
        static void handleMQTTSettings(AsyncWebServerRequest* request);               
        static void handleUpdateTimings(AsyncWebServerRequest* request);
        static void handleUpdateHostname(AsyncWebServerRequest* request);
        static void handleForgetWiFi(AsyncWebServerRequest* request);
        static void handleResetSettings(AsyncWebServerRequest* request);

        static String currentSensorJson;
        
        static SettingsManager* settingsManager;    
};

#endif