#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWiFiManager.h>
#include "WebServer.h"
#include "Serverpages/AllPages.h"

// globals
AsyncWebServer server(80);
AsyncWebSocket webSocket("/ws");
AsyncEventSource events("/events");

String WebServer::currentSensorJson = "";

SettingsManager* WebServer::settingsManager;    

static const char NAV_BAR[] PROGMEM = 
    "<nav class='navbar navbar-expand-sm bg-dark navbar-dark fixed-top'>"
    "<a class='navbar-brand' href='index.html'>Room Sensor</a>"
    "<ul class='navbar-nav'>"
    "<li class='nav-item'>"
    "<a class='nav-link' href='settings.html'>Settings</a>"
    "</li>"
    "<li class='nav-item'>"
    "<a class='nav-link' href='thingSpeakSettings.html'>ThingSpeak</a>"
    "</li>"
    "<li class='nav-item'>"
    "<a class='nav-link' href='mqttSettings.html'>MQTT</a>"
    "</li>"
    "</nav>";

// methods

void WebServer::init(SettingsManager* settingsManager)
{
    this->settingsManager = settingsManager;
    
    webSocket.onEvent(onEvent);
    server.addHandler(&webSocket);
    server.addHandler(&events);

// SPIFFS for reference
//request->send(SPIFFS, "/settings.html", String(), false, tokenProcessor);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "text/html", index_html, tokenProcessor);
        //request->send(SPIFFS, "/index.html", String(), false, tokenProcessor);
    });

    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {        
        request->send_P(200, "text/html", index_html, tokenProcessor);
        //request->send(SPIFFS, "/index.html", String(), false, tokenProcessor);
    });

    server.on("/settings.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {        
        request->send_P(200, "text/html", settings_html, tokenProcessor);
        //request->send(SPIFFS, "/settings.html", String(), false, tokenProcessor);
    });

    server.on("/thingSpeakSettings.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "text/html", thingSpeakSettings_html, tokenProcessor);
    });

    server.on("/mqttSettings.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "text/html", mqttSettings_html, tokenProcessor);        
        //request->send(SPIFFS, "/mqttSettings.html", String(), false, tokenProcessor);
    });

    server.on("/updateThingSpeak.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        handleUpdateThingSpeakSettings(request);
        request->redirect("/index.html");
    });

    server.on("/updateMqttSettings.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        handleMQTTSettings(request);
        request->redirect("/index.html");
    });

    server.on("/updateTimings.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        handleUpdateTimings(request);
        request->redirect("/index.html");
    });

    server.on("/updateHostname.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        handleUpdateHostname(request);
        request->redirect("/index.html");
    });

    server.on("/resetSettings.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        handleResetSettings(request);
        request->redirect("/index.html");
    });

    server.on("/forgetWiFi.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->redirect("/index.html");
        handleForgetWiFi(request);
    });

    server.on("/js/station.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "application/javascript", station_js);
        //request->send(SPIFFS, "/js/station.js", String(), false, tokenProcessor);
    });

    server.on("/js/settings.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "application/javascript", settings_js);
    });

    server.on("/js/thingSpeakSettings.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "application/javascript", thingSpeakSettings_js);
    });

    server.on("/js/jquery.confirmModal.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "application/javascript", confirmModal_js);
    });

    server.on("/js/mqttSettings.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "application/javascript", mqttSettings_js);
    });

    server.on("/css/station.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "text/css", station_css);
    });

    server.begin();
}

AsyncWebServer* WebServer::getServer()
{
    return &server;
}

void WebServer::updateSensorReadings(float temp, float humidity, float pressure)
{
    String output;

    DynamicJsonDocument jsonDoc(512);   // TODO

    jsonDoc["type"] = "sensorReadings";

    JsonObject readings = jsonDoc.createNestedObject("readings");
    readings["temp"] = temp;
    readings["humidity"] = humidity;
    readings["pressure"] = pressure;
    readings["hostname"] = settingsManager->getHostname();

    serializeJson(jsonDoc, output);
    currentSensorJson = output;

    if(webSocket.count() > 0)
    {
        //Serial.println("Sending sensor readings to cilent");
        webSocket.textAll(output);
    }
}

void WebServer::updateClientOnConnect()
{
    //Serial.println("Client connected, sending any available data.");

    if(currentSensorJson.length() != 0)
    {
        webSocket.textAll(currentSensorJson);
    }
}

void WebServer::onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{
    if(type == WS_EVT_CONNECT)
    {
        //Serial.println("Websocket connect");
        updateClientOnConnect();
    }
}

String WebServer::tokenProcessor(const String& token)
{
    if(token == "NAVBAR")
    {
        return FPSTR(NAV_BAR);
    }
    if(token == "THINGSPEAKCHANNELKEY")    
    {
        return String(settingsManager->getThingSpeakChannelID());
    }
    if(token == "THINGSPEAKAPIKEY")    
    {
        return settingsManager->getThingSpeakApiKey();
    }
    if(token == "SENSORREADINGINTERVAL")    
    {
        return String(settingsManager->getSensorReadingInterval() / SECONDS_MULT);
    }
    if(token == "THINGSPEAKREPORTINGINTERVAL")    
    {
        return String(settingsManager->getThingSpeakReportingInterval() / SECONDS_MULT);
    }   
    if(token == "THINGSPEAKENABLED")
    {
        if(settingsManager->getThingSpeakEnabled() == true)
        {
            return "Checked";
        }
    }
    if(token == "MQTTPUBLISHINTERVAL")    
    {
        return String(settingsManager->getMqttPublishInterval() / SECONDS_MULT);
    }    
    if(token == "MQTTENABLED")
    {
        if(settingsManager->getMqttEnabled() == true)
        {
            return "Checked";
        }
    }
    if(token == "MQTTBROKERURL")
    {
        return settingsManager->getMqttBroker();
    }
    if(token == "MQTTUSERNAME")
    {
        return settingsManager->getMqttUsername();
    }
    if(token == "MQTTPASSWORD")
    {
        return settingsManager->getMqttPassword();
    }
    if(token == "MQTTCLIENTID")
    {
        return settingsManager->getMqttClientId();
    }    
    if(token == "MQTTTEMPTOPIC")
    {
        return settingsManager->getMqttTempTopic();
    }
    if(token == "MQTTHUMIDITYTOPIC")
    {
        return settingsManager->getMqttHumidityTopic();
    }
    if(token == "MQTTPRESSURETOPIC")
    {
        return settingsManager->getMqttPressureTopic();
    }  
    if(token == "MQTTPORT")
    {
        return String(settingsManager->getMqttPort());
    }
    if(token == "HOSTNAME")
    {
        return settingsManager->getHostname();
    }  

    return String();
}

void WebServer::handleUpdateThingSpeakSettings(AsyncWebServerRequest* request)
{
    if(request->hasParam("thingSpeakApiKey"))
    {
        AsyncWebParameter* p = request->getParam("thingSpeakApiKey");
        settingsManager->setThingSpeakApiKey(p->value());

        //Serial.println("Got ThingSpeak API key of: " + p->value());
    }
    if(request->hasParam("thingSpeakChannel"))
    {
        AsyncWebParameter* p = request->getParam("thingSpeakChannel");
        settingsManager->setThingSpeakChannelID(p->value().toInt());

        //Serial.println("Got ThingSpeak channel ID of: " + p->value());
    }
    if(request->hasParam("thingSpeakEnabled"))
    {        
        settingsManager->setThingSpeakEnabled(true);
    }
    else
    {
        settingsManager->setThingSpeakEnabled(false);
    }    
}

void WebServer::handleMQTTSettings(AsyncWebServerRequest* request)
{
    if(request->hasParam("mqttEnabled"))
    {        
        settingsManager->setMqttEnabled(true);
    }
    else
    {
        settingsManager->setMqttEnabled(false);
    }

    if(request->hasParam("mqttBroker"))
    {
        AsyncWebParameter* p = request->getParam("mqttBroker");
        settingsManager->setMqttBroker(p->value());
    }
    if(request->hasParam("mqttPort"))
    {
        AsyncWebParameter* p = request->getParam("mqttPort");
        settingsManager->setMqttPort(p->value().toInt());
    }
    if(request->hasParam("mqttUsername"))
    {
        AsyncWebParameter* p = request->getParam("mqttUsername");
        settingsManager->setMqttUsername(p->value());
    }
    if(request->hasParam("mqttPassword"))
    {
        AsyncWebParameter* p = request->getParam("mqttPassword");
        settingsManager->setMqttPassword(p->value());
    }
    if(request->hasParam("mqttClientId"))
    {
        AsyncWebParameter* p = request->getParam("mqttClientId");
        settingsManager->setMqttClientId(p->value());
    }
    if(request->hasParam("mqttTempTopic"))
    {
        AsyncWebParameter* p = request->getParam("mqttTempTopic");
        settingsManager->setMqttTempTopic(p->value());
    }
    if(request->hasParam("mqttHumidityTopic"))
    {
        AsyncWebParameter* p = request->getParam("mqttHumidityTopic");
        settingsManager->setMqttHumidityTopic(p->value());
    }
    if(request->hasParam("mqttPressureTopic"))
    {
        AsyncWebParameter* p = request->getParam("mqttPressureTopic");
        settingsManager->setMqttPressureTopic(p->value());
    }
}


void WebServer::handleUpdateTimings(AsyncWebServerRequest* request)
{
    if(request->hasParam("sensorReadingInterval"))
    {
        AsyncWebParameter* p = request->getParam("sensorReadingInterval");
        settingsManager->setSensorReadingInterval(p->value().toInt() * SECONDS_MULT);
    }
    if(request->hasParam("thingSpeakReportingInterval"))
    {
        AsyncWebParameter* p = request->getParam("thingSpeakReportingInterval");
        settingsManager->setThingSpeakReportingInterval(p->value().toInt() * SECONDS_MULT);
    }
    if(request->hasParam("mqttPublishInterval"))
    {
        AsyncWebParameter* p = request->getParam("mqttPublishInterval");
        settingsManager->setMqttPublishInterval(p->value().toInt() * SECONDS_MULT);
    }
}

void WebServer::handleUpdateHostname(AsyncWebServerRequest* request)
{
    if(request->hasParam("hostName"))
    {
        AsyncWebParameter* p = request->getParam("hostName");
        settingsManager->setHostname(p->value());
    }
}

void WebServer::handleForgetWiFi(AsyncWebServerRequest* request)
{
    DNSServer dns;
    AsyncWiFiManager wifiManager(getServer(), &dns);
    wifiManager.resetSettings();
    ESP.restart();
}

void WebServer::handleResetSettings(AsyncWebServerRequest* request)
{
    settingsManager->resetSettings();
}
