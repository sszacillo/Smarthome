#ifndef WIFI_MQTT_H
#define WIFI_MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>

extern PubSubClient mqttClient;

void setup_wifi_mqtt();
void mqtt_reconnect();
void mqtt_callback(char* topic, unsigned char* payload, unsigned int length);
void taskMQTTLoop(void *parameter);

#endif
