#include <Arduino.h>
#include "wifi_mqtt.h"
#include "led_control.h"
#include "sensors.h"
#include "tasks.h"

void setup() {
    Serial.begin(9600);

    // Inicjalizacja WiFi, MQTT i sensorów
    setup_wifi_mqtt();
    setup_leds();
    setup_sensors();
    // Tworzenie zadań FreeRTOS
    xTaskCreate(taskReadSensors, "ReadSensors", 2048, NULL, 1, NULL);
    xTaskCreate(taskHandleLEDs, "HandleLEDs", 2048, NULL, 1, NULL);
    xTaskCreate(taskMQTTLoop, "MQTTLoop", 2048, NULL, 1, NULL);
}

void loop() {
    
}
