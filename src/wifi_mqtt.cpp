#include "wifi_mqtt.h"
#include "config.h"
#include "led_control.h"  // Include to access handle_mqtt_message
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup_wifi_mqtt() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi");
    mqttClient.setServer(mqtt_server, mqtt_port);
    mqttClient.setCallback(mqtt_callback);
}

void mqtt_reconnect() {
    while (!mqttClient.connected()) {
        if (mqttClient.connect("ESP32Client")) {
            mqttClient.subscribe(mqtt_topic_led);
            mqttClient.subscribe("smarthome/settings/light_sensor_sensitivity/data");
        } else {
            delay(5000);
        }
    }
}

void mqtt_callback(char* topic, unsigned char* payload, unsigned int length) {
    handle_mqtt_message(topic, payload, length);  // Now visible due to the included header
}

void taskMQTTLoop(void *parameter) {
    while (true) {
        if (!mqttClient.connected()) {
            mqtt_reconnect();
        }
        mqttClient.loop();
        vTaskDelay(100/ portTICK_PERIOD_MS);
    }
}
