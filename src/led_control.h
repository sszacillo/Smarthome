#ifndef LED_CONTROL_H
#define LED_CONTROL_H
#include "config.h"
#include <Arduino.h>
void setup_leds();
void setColor(uint8_t ledIndex, uint8_t red, uint8_t green, uint8_t blue);
void handle_mqtt_message(char* topic, unsigned char* payload, unsigned int length);
void taskHandleLEDs(void *parameter);

#endif
