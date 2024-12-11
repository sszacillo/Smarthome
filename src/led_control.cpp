#include "config.h"
#include "led_control.h"
#include "sensors.h"
#include "wifi_mqtt.h"
#include <FastLED.h>
#include <ArduinoJson.h>
#include <Adafruit_TSL2591.h>

extern Adafruit_TSL2591 tsl;
extern SemaphoreHandle_t tslMutex;

uint16_t luminosity = 0;
float luminosityThreshold = 500;
CRGB leds[NUM_LEDS];
bool lastLedState = false;

void setup_leds() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void setColor(uint8_t ledIndex, uint8_t red, uint8_t green, uint8_t blue) {
    if (ledIndex < NUM_LEDS) {
        leds[ledIndex] = CRGB(red, green, blue);
        FastLED.show();
    }
}

void handle_mqtt_message(char* topic, unsigned char* payload, unsigned int length) {
    String messageTemp;
    for (int i = 0; i < length; i++) {
        messageTemp += (char)payload[i];
    }

    String topicLum = "smarthome/settings/light_sensor_sensitivity/data";


    StaticJsonDocument<200> doc;
    if (deserializeJson(doc, messageTemp) == DeserializationError::Ok) {
        if (String(topic) == topicLum) {
            Serial.print(topicLum);
            luminosityThreshold = doc["value"].as<int>();
            Serial.print(luminosityThreshold);
        } else {
            String topicStr = String(topic);
            Serial.print(topic);
            int startIndex = topicStr.indexOf("LED/") + 4;
            int endIndex = topicStr.indexOf("/data");
            String ledNumberStr = topicStr.substring(startIndex, endIndex);
            int ledNumber = ledNumberStr.toInt() - 1;

            int r = doc["red"];
            int g = doc["green"];
            int b = doc["blue"];

            if (ledNumber >= 0 && ledNumber < NUM_LEDS - 1) {
                setColor(ledNumber, r, g, b);
            }
        }
    } else {
        Serial.println("Error parsing JSON");
    }
}

void setLastLed(bool state) {
    if (state != lastLedState) {
        leds[NUM_LEDS - 1] = state ? CRGB::White : CRGB::Black;
        FastLED.show();
        lastLedState = state;

        // Create a JSON object for the status message
        StaticJsonDocument<100> statusDoc;
        statusDoc["status"] = state ? "on" : "off";
        char jsonBuffer[100];
        serializeJson(statusDoc, jsonBuffer);

        // Publish the JSON message to the MQTT topic
        mqttClient.publish("led/6/status", jsonBuffer);
    }
}

void taskHandleLEDs(void *parameter) {
    while (true) {
        if (xSemaphoreTake(tslMutex, portMAX_DELAY)) {
            luminosity = tsl.getLuminosity(TSL2591_VISIBLE);
            xSemaphoreGive(tslMutex);
            setLastLed(luminosity < luminosityThreshold);
        } else {
            Serial.println("Failed to acquire TSL2591 mutex in taskHandleLEDs.");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
