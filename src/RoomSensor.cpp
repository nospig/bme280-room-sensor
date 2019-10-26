#include <Arduino.h>
#include <ArduinoOTA.h>
#include <TaskScheduler.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWifiManager.h>
#include "SettingsManager.h"
#include "Settings.h"
#include "RoomSensor.h"
#include "WebServer.h"
#include "ThingSpeakReporter.h"
#include "BMEReader.h"
#include "MQTTManager.h"
#include <FS.h>

// globals
Scheduler taskScheduler;
WebServer webServer;
ThingSpeakReporter thingSpeakReporter;
DisplayBase* display;
SettingsManager settingsManager;
DNSServer dns;
BMEReader bmeReader;
MQTTManager mqttManager;

float sensorTemp = 0, sensorHumidity = 0, sensorPressure = 0;

// tasks
Task connectWifi(0, TASK_ONCE, &connectWifiCallback);
Task readSensors(60*SECONDS_MULT, TASK_FOREVER, &readSensorsCallback);
Task updateThingSpeak(60*SECONDS_MULT, TASK_FOREVER, &updateThingSpeakCallback);
Task updateWiFiStrength(WIFI_STRENGTH_INTERVAL, TASK_FOREVER, &updateWifiStrengthCallback);
Task checkSettingsChanged(SETTINGS_CHANGED_INTERVAL, TASK_FOREVER, &checkSettingsChangedCallback);
Task mqttPublish(5*MINUTES_MULT, TASK_FOREVER, &mqttPublishCallback);

// task callbacks

// sensors

void readSensorsCallback()
{
    if(bmeReader.isActive())
    {
        sensorTemp = bmeReader.readTemp();
        sensorHumidity = bmeReader.readHumidity();
        sensorPressure = bmeReader.readPressurehPA();

        display->drawSensorReadings(sensorTemp, sensorHumidity, sensorPressure);
        webServer.updateSensorReadings(sensorTemp, sensorHumidity, sensorPressure);
        
        // only enable thing speak and mqtt after some data recorded
        updateThingSpeak.enableIfNot();
        mqttPublish.enableIfNot();
    }
}

// ThingSpeak

void updateThingSpeakCallback()
{
    // just send the latest sensor saved readings, no need to update again, avoid chances of updating the sensor too soon
    if(settingsManager.getThingSpeakApiKey() != "" && settingsManager.getThingSpeakEnabled())
    {
        thingSpeakReporter.sendSensorReadings(sensorTemp, sensorHumidity, sensorPressure);
    }
}

// MQTT

void mqttPublishCallback()
{
    // just send latest readings
    mqttManager.updateSensorReadings(sensorTemp, sensorHumidity, sensorPressure);
}


// wifi

void connectWifiCallback()
{
    AsyncWiFiManager wifiManager(webServer.getServer(), &dns);

    settingsManager.init();

    display->drawStartupDisplay();

    wifiManager.autoConnect("Room Station");

    setupOtaUpdates();

    Serial.println(WiFi.localIP());

    webServer.init(&settingsManager);
    thingSpeakReporter.init(&settingsManager);
    bmeReader.init(BME_SDA, BME_SCL, BME_ADDRESS);
    mqttManager.init(&settingsManager);
    mqttManager.setSubscribeCallback(mqttSubscribeCallback);

    delay(WIFI_CONNECTING_DELAY);
    display->startMainDisplay();

    taskScheduler.addTask(readSensors);
    taskScheduler.addTask(updateThingSpeak);
    taskScheduler.addTask(updateWiFiStrength);
    taskScheduler.addTask(checkSettingsChanged);
    taskScheduler.addTask(mqttPublish);

    // timings
    readSensors.setInterval(settingsManager.getSensorReadingInterval());
    updateThingSpeak.setInterval(settingsManager.getThingSpeakReportingInterval());
    mqttPublish.setInterval(settingsManager.getMqttPublishInterval());

    updateThingSpeak.disable(); 
    mqttPublish.disable();
    readSensors.enable();
    updateWiFiStrength.enable();
    checkSettingsChanged.enable();
}

void updateWifiStrengthCallback()
{
    long wifiStrength = WiFi.RSSI();
    display->drawWiFiStrength(wifiStrength);
}

// settings

void checkSettingsChangedCallback()
{
    if(settingsManager.getSettingsChanged())
    {
        //Serial.println("Settings changed.");
        settingsManager.resetSettingsChanged();

        if(settingsManager.getMqttReconnectRequired())
        {
            mqttManager.reconnect();
            settingsManager.resetMqttReconnectRequired();
        }

        // best just to force a display clear when changing settings
        display->restartMainDisplay();

        readSensors.setInterval(settingsManager.getSensorReadingInterval());
        updateThingSpeak.setInterval(settingsManager.getThingSpeakReportingInterval());
        mqttPublish.setInterval(settingsManager.getMqttPublishInterval());

        readSensors.forceNextIteration();
        updateWiFiStrength.forceNextIteration();
        updateThingSpeak.forceNextIteration();
        mqttPublish.forceNextIteration();
    }
}

// mqtt

void mqttSubscribeCallback(const char* topic, const char *payload)
{
    if(!strcmp(topic, settingsManager.getMqttDisplayTopic().c_str()))
    {
        if(!strcmp(payload, "on"))
        {
            //Serial.println("Display on");
            display->setDisplayEnabled(true);
        }
        if(!strcmp(payload, "off"))
        {
            //Serial.println("Display off");
            display->setDisplayEnabled(false);
        }
    }
}

// basic setup and loop

void setup() 
{
    Serial.begin(115200);

    SPIFFS.begin();

    //display = new DisplaySerial();
    display = new DisplayBase();

    WiFi.hostname("Room Sensor");

    taskScheduler.startNow(); 
    taskScheduler.addTask(connectWifi);
    connectWifi.enable();
}

void loop() 
{
    taskScheduler.execute();
    ArduinoOTA.handle();
}

// OTA, not through webserver for now

void setupOtaUpdates()
{
    // set MD5 hash password here
    // add a file to the project called uploadPassword.ini
    // make it's contents
    // [password]
    // upload_password = password
    // where password is plain text
    ArduinoOTA.setPasswordHash(OTA_MD5_PASSWORD_HASH);

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
        {
            Serial.println("Start updating firmware.");
        }
        else
        {
            SPIFFS.end();
            Serial.println("Start updating file system.");
        }
    });

    ArduinoOTA.onEnd([]() 
    {
        Serial.println("\nEnd");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) 
    {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
        {
            Serial.println("Auth Failed");
        }
        else if (error == OTA_BEGIN_ERROR)
        {
            Serial.println("Begin Failed");
        }
        else if (error == OTA_CONNECT_ERROR)
        {
            Serial.println("Connect Failed");
        }
        else if (error == OTA_RECEIVE_ERROR)
        {
            Serial.println("Receive Failed");
        }
        else if (error == OTA_END_ERROR)
        {
            Serial.println("End Failed");
        }
    });

    ArduinoOTA.begin();
}