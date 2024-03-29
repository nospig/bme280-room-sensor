#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include "SettingsManager.h"

const int SENSOR_READING_INTERVAL       = 30 * SECONDS_MULT;
const int THINGSPEAK_REPORTING_INTERVAL = 30 * MINUTES_MULT;
const int MQTT_PUBLISH_INTERVAL         = 5 * MINUTES_MULT;

void SettingsManager::init()
{
    // for testing now as settings not saved to start with
    //SPIFFS.remove(SETTINGS_FILE_NAME);

    if(!SPIFFS.exists(SETTINGS_FILE_NAME))
    {
        //Serial.println("No settings found, creating.");
        resetSettings();
        saveSettings();
    }
    
    loadSettings();

    settingsChanged = false;
    mqttReconnectRequired = false;
}

void SettingsManager::resetSettings()
{
    data.thingSpeakChannelID = 0;
    data.thingSpeakAPIKey = "";

    data.sensorReadingInterval = SENSOR_READING_INTERVAL;
    data.thingSpeakReportingInterval = THINGSPEAK_REPORTING_INTERVAL;
    data.mqttPublishInterval = MQTT_PUBLISH_INTERVAL;

    data.thingSpeakEnabled = false;
    data.mqttEnabled = false;

    data.mqttBroker = "";
    data.mqttUsername = "";
    data.mqttPassword = "";
    data.mqttClientId = "";
    data.mqttTempTopic = "";
    data.mqttHumidityTopic = "";
    data.mqttPressureTopic = "";
    data.mqttPort = 1883;

    data.hostname = "Sensor";

    saveSettings();
    settingsChanged = true;
}

void SettingsManager::loadSettings()
{
    File jsonSettings;
    DynamicJsonDocument doc(SETTINGS_JSON_SIZE);

    jsonSettings = SPIFFS.open(SETTINGS_FILE_NAME, "r");
    deserializeJson(doc, jsonSettings);

    data.thingSpeakChannelID = doc["ThingSpeakChannelID"];
    data.thingSpeakAPIKey = (const char*)doc["ThingSpeakAPIKey"];

    data.sensorReadingInterval = doc["SensorReadingInterval"];
    data.thingSpeakReportingInterval = doc["ThingSpeakReportingInterval"];
    data.mqttPublishInterval = doc["MqttPublishInterval"];

    data.thingSpeakEnabled = doc["ThingSpeakEnabled"];
    data.mqttEnabled = doc["MqttEnabled"];

    data.mqttBroker = (const char*)doc["MqttBroker"];
    data.mqttUsername = (const char*)doc["MqttUsername"];
    data.mqttPassword = (const char*)doc["MqttPassword"];
    data.mqttClientId = (const char*)doc["MqttClientID"];
    data.mqttTempTopic = (const char*)doc["MqttTempTopic"];
    data.mqttHumidityTopic = (const char*)doc["MqttHumidityTopic"];
    data.mqttPressureTopic = (const char*)doc["MqttPressureTopic"];
    data.mqttPort = doc["MqttPort"];    
    data.hostname = (const char*)doc["Hostname"];

    // testing
    Serial.println();
    serializeJson(doc, Serial);
    Serial.println();
}

void SettingsManager::saveSettings()
{
    File jsonSettings;
    DynamicJsonDocument doc(SETTINGS_JSON_SIZE);   

    doc["ThingSpeakChannelID"] = data.thingSpeakChannelID;
    doc["ThingSpeakAPIKey"] = data.thingSpeakAPIKey;
    doc["SensorReadingInterval"] = data.sensorReadingInterval;
    doc["ThingSpeakReportingInterval"] = data.thingSpeakReportingInterval;
    doc["ThingSpeakEnabled"] = data.thingSpeakEnabled;
    doc["MqttPublishInterval"] = data.mqttPublishInterval;
    doc["MqttEnabled"] = data.mqttEnabled;
    doc["MqttBroker"] = data.mqttBroker;
    doc["MqttUsername"] = data.mqttUsername;
    doc["MqttPassword"] = data.mqttPassword;
    doc["MqttClientID"] = data.mqttClientId;
    doc["MqttTempTopic"] = data.mqttTempTopic;
    doc["MqttHumidityTopic"] = data.mqttHumidityTopic;
    doc["MqttPressureTopic"] = data.mqttPressureTopic;
    doc["MqttPort"] = data.mqttPort;
    doc["Hostname"] = data.hostname;

    jsonSettings = SPIFFS.open(SETTINGS_FILE_NAME, "w");
    if(jsonSettings)
    {
        serializeJson(doc, jsonSettings);
        //Serial.println("Settings file saved.");
        jsonSettings.close();
    }
    else
    {
        Serial.println("Unable to save settings file.");
    }
    
    // testing
    //serializeJson(doc, Serial); 
    //Serial.println();
}

void SettingsManager::updateSettings()
{
    saveSettings();
    settingsChanged = true;
}

String SettingsManager::getThingSpeakApiKey()
{
    return data.thingSpeakAPIKey;
}

void SettingsManager::setThingSpeakApiKey(String apiKey)
{
    if(apiKey != data.thingSpeakAPIKey)
    {
        data.thingSpeakAPIKey = apiKey;
        updateSettings();
    }
}

int SettingsManager::getThingSpeakChannelID()
{
    return data.thingSpeakChannelID;
}

void SettingsManager::setThingSpeakChannelID(int channelID)
{
    if(channelID != data.thingSpeakChannelID)
    {
        data.thingSpeakChannelID = channelID;
        updateSettings();
    }
}

int SettingsManager::getSensorReadingInterval()
{
    return data.sensorReadingInterval;
}

void SettingsManager::setSensorReadingInterval(int interval)
{
    if(data.sensorReadingInterval != interval)
    {
        data.sensorReadingInterval = interval;
        updateSettings();
    }
}

int SettingsManager::getThingSpeakReportingInterval()
{
    return data.thingSpeakReportingInterval;
}

void SettingsManager::setThingSpeakReportingInterval(int interval)
{
    if(data.thingSpeakReportingInterval != interval)
    {
        data.thingSpeakReportingInterval = interval;
        updateSettings();
    }
}

bool SettingsManager::getThingSpeakEnabled()
{
    return data.thingSpeakEnabled;
}

void SettingsManager::setThingSpeakEnabled(bool enabled)
{
    if(data.thingSpeakEnabled != enabled)
    {
        data.thingSpeakEnabled = enabled;
        updateSettings();
    }
}

int SettingsManager::getMqttPublishInterval()
{
    return data.mqttPublishInterval;
}

void SettingsManager::setMqttPublishInterval(int interval)
{
    if(data.mqttPublishInterval != interval)
    {
        data.mqttPublishInterval = interval;
        updateSettings();
    }
}

bool SettingsManager::getMqttEnabled()
{
    return data.mqttEnabled;
}

void SettingsManager::setMqttEnabled(bool enabled)
{
    if(data.mqttEnabled != enabled)
    {
        data.mqttEnabled = enabled;
        updateSettings();

        if(enabled)
        {
            mqttReconnectRequired = true;
        }
    }
}

int SettingsManager::getMqttPort()
{
    return data.mqttPort;
}

void SettingsManager::setMqttPort(int port)
{
    if(data.mqttPort != port)
    {
        data.mqttPort = port;
        updateSettings();
        mqttReconnectRequired = true;
    }
}

String SettingsManager::getMqttBroker()
{
    return data.mqttBroker;
}

void SettingsManager::setMqttBroker(String url)
{
    if(data.mqttBroker != url)
    {
        data.mqttBroker = url;
        updateSettings();
        mqttReconnectRequired = true;
    }
}

String SettingsManager::getMqttUsername()
{
    return data.mqttUsername;
}

void SettingsManager::setMqttUsername(String userName)
{
    if(data.mqttUsername != userName)
    {
        data.mqttUsername = userName;
        updateSettings();
        mqttReconnectRequired = true;
    }
}

String SettingsManager::getMqttPassword()
{
    return data.mqttPassword;
}

void SettingsManager::setMqttPassword(String password)
{
    if(data.mqttPassword != password)
    {
        data.mqttPassword = password;
        updateSettings();
        mqttReconnectRequired = true;
    }
}

String SettingsManager::getMqttClientId()
{
    return data.mqttClientId;
}

void SettingsManager::setMqttClientId(String clientId)
{
    if(data.mqttClientId != clientId)
    {
        data.mqttClientId = clientId;
        updateSettings();
    }
}

String SettingsManager::getMqttTempTopic()
{
    return data.mqttTempTopic;
}

void SettingsManager::setMqttTempTopic(String tempTopic)
{
    if(data.mqttTempTopic != tempTopic)
    {
        data.mqttTempTopic = tempTopic;
        updateSettings();
    }
}

String SettingsManager::getMqttHumidityTopic()
{
    return data.mqttHumidityTopic;
}

void SettingsManager::setMqttHumidityTopic(String humidityTopic)
{
    if(data.mqttHumidityTopic != humidityTopic)
    {
        data.mqttHumidityTopic = humidityTopic;
        updateSettings();
    }
}

String SettingsManager::getMqttPressureTopic()
{
    return data.mqttPressureTopic;
}

void SettingsManager::setMqttPressureTopic(String pressureTopic)
{
    if(data.mqttPressureTopic != pressureTopic)
    {
        data.mqttPressureTopic = pressureTopic;
        updateSettings();
    }
}

String SettingsManager::getHostname()
{
    return data.hostname;
}

void SettingsManager::setHostname(String hostname)
{
    if(data.hostname != hostname)
    {
        data.hostname = hostname;
        updateSettings();
    }
}

bool SettingsManager::getSettingsChanged()
{
    return settingsChanged;
}

void SettingsManager::resetSettingsChanged()
{
    settingsChanged = false;
}

bool SettingsManager::getMqttReconnectRequired()
{
    return mqttReconnectRequired;
}

void SettingsManager::resetMqttReconnectRequired()
{
    mqttReconnectRequired = false;
}
