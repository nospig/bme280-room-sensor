# BME280 room sensor

A simple ESP8266 based project to use a BME280 to measure room conditions.

## Features

* Creates an access point for connecting to any WiFi network.
* BME280 sensor for local temperature, pressure and humidity readings.
* Uses a cheap ESP8266 board as the controller.
* All settings controlled through a web interface which also displays sensor readings.
* ThingSpeak integration - send sensor readings.
* MQTT - publishes sensor readings.
* Password protected OTA updates enabled through PlatformIO (not through web interface).


## Libraries Used

* Adafruit BME280
* ArduinoJson
* ESPAsyncWiFiManager
* ESPAsyncTCP
* ESPAsyncWebServer
* TaskScheduler
* ThingSpeak
* AsyncMqttClient

## Hardware

Built using the following hardware from AliExpress.

[Wemos D1 mini ESP8266 board](https://www.aliexpress.com/item/32651747570.html)  
[BME280 sensor](https://www.aliexpress.com/item/32849462236.html)

## Wiring

    Mini D1  -> BME SCL  
    Mini D2  -> BME SDA  

## UserSettings.h

Contains defines for MD5 password hash if you want to use OTA for future updates.

## OTA updates

Password protected OTA updates enabled. Set the MD5 hash of your prefered password in UserSettings.h. To upload via IP in PlatformIO create a file called uploadPassword.ini in the same folder as platformio.ini with the following contents:

    [password]
    upload_password = xxxxx
    
Replace xxxxx with the plain text version of your MD5 hash set above.

## MQTT

The station can publish sensor readings to a MQTT broker. Enter broker details on the MQTT settings page through the webserver. Use packages such as Home Assistant, Node-Red, Influxdb and Grafana to graph local conditions.

## ThingSpeak

Sensor readings can be published to a ThingSpeak channel. Enable and set your ThingSpeak details in the ThingSpeak settings page.

## Case

Custom 3D printed case currently in development.

