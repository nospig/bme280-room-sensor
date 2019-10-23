#ifndef _settingsmanager_h
#define _settingsmanager_h

#include <ArduinoJson.h>
#include "DisplayBase.h"
#include "Settings.h"

const String SETTINGS_FILE_NAME = "/Settings.json";
const int SETTINGS_JSON_SIZE = 1536;

typedef struct SettingsData
{
    int thingSpeakChannelID;
    String thingSpeakAPIKey;
    
    int sensorReadingInterval;
    int thingSpeakReportingInterval;
    int mqttPublishInterval;

    String mqttBroker;
    String mqttUsername;
    String mqttPassword;
    String mqttClientId;
    String mqttTempTopic;
    String mqttHumidityTopic;
    String mqttPressureTopic;
    String mqttDisplayTopic;
    int mqttPort;

    bool thingSpeakEnabled;
    bool mqttEnabled;
} SettingsData;

class SettingsManager
{
    public:
        void init();

        void resetSettings();

        String getThingSpeakApiKey();
        void setThingSpeakApiKey(String apiKey);

        int getThingSpeakChannelID();
        void setThingSpeakChannelID(int channelID);

        int getSensorReadingInterval();
        void setSensorReadingInterval(int interval);

        int getThingSpeakReportingInterval();
        void setThingSpeakReportingInterval(int interval);

        bool getThingSpeakEnabled();
        void setThingSpeakEnabled(bool enabled);

        int getMqttPublishInterval();
        void setMqttPublishInterval(int interval);

        bool getMqttEnabled();
        void setMqttEnabled(bool enabled);

        int getMqttPort();
        void setMqttPort(int port);

        String getMqttBroker();
        void setMqttBroker(String url);

        String getMqttUsername();
        void setMqttUsername(String userName);

        String getMqttPassword();
        void setMqttPassword(String password);

        String getMqttClientId();
        void setMqttClientId(String clientId);

        String getMqttTempTopic();
        void setMqttTempTopic(String tempTopic);

        String getMqttHumidityTopic();
        void setMqttHumidityTopic(String humidityTopic);

        String getMqttPressureTopic();
        void setMqttPressureTopic(String pressureTopic);

        String getMqttDisplayTopic();
        void setMqttDisplayTopic(String displayTopic);

        bool getMqttReconnectRequired();
        void resetMqttReconnectRequired();

        bool getSettingsChanged();
        void resetSettingsChanged();

    private:
        SettingsData data;
        bool settingsChanged;
        bool mqttReconnectRequired;

        void loadSettings();
        void saveSettings();
        void updateSettings();
};

#endif // _settingsmanager_h