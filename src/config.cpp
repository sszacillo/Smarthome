#include "config.h"

// Network and MQTT Configuration Definitions
const char* ssid = "OnePlus 9 Pro";
const char* password = "12345678";
const char* mqtt_server = "156.17.231.93";
const int mqtt_port = 1883;


// Power and Measurement Global Variables
float extMeterReading_mA = 1000.0;
float ina219Reading_mA = 1000.0;


// MQTT topics
const char* mqtt_topic_led = "smarthome/energy/LED/#";
const char* mqtt_topic_power = "smarthome/energy/powerconsumption/";
const char* mqtt_topic_light = "smarthome/light/intensity/data";
const char* topic_temperature = "smarthome/environment/multi_sensor/temperature/data";
const char* topic_humidity = "smarthome/environment/multi_sensor/humidity/data";
const char* topic_pressure = "smarthome/environment/multi_sensor/pressure/data";
const char* topic_voltage_supply = "smarthome/energy/energy_consumption/voltage/supply/data";
const char* topic_voltage_bus = "smarthome/energy/energy_consumption/voltage/bus/data";
const char* topic_current = "smarthome/energy/energy_consumption/current/data";
const char* topic_power = "smarthome/energy/energy_consumption/power/data";