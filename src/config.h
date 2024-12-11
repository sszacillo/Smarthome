#ifndef CONFIG_H
#define CONFIG_H

// Network and MQTT Configuration
extern const char* ssid;
extern const char* password;
extern const char* mqtt_server;
extern const int mqtt_port;  // Declared as const int

// MQTT topics
extern const char* mqtt_topic_led;
extern const char* mqtt_topic_power;
extern const char* mqtt_topic_light;
extern const char* mqtt_topic_light;
extern const char* topic_temperature;
extern const char* topic_humidity;
extern const char* topic_pressure;
extern const char* topic_voltage_supply;
extern const char* topic_voltage_bus;
extern const char* topic_current;
extern const char* topic_power;


// Power and Measurement Configuration
extern float extMeterReading_mA;
extern float ina219Reading_mA;

// LED Configuration
#define NUM_LEDS 3
#define LED_PIN 2
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define BRIGHTNESS 255

#endif  // CONFIG_H
