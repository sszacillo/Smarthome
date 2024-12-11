#include "sensors.h"
#include <Adafruit_TSL2591.h>
#include <DFRobot_INA219.h>
#include "config.h" // Include MQTT topic variables and other configurations
#include "wifi_mqtt.h"
#include <Adafruit_BME280.h>

// Initialize sensors
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
DFRobot_INA219_IIC ina219Primary(&Wire, INA219_I2C_ADDRESS4);  // First INA219 sensor
DFRobot_INA219_IIC ina219Secondary(&Wire, INA219_I2C_ADDRESS2);  // Second INA219 sensor for voltage measurement only
SemaphoreHandle_t tslMutex;
Adafruit_BME280 bme; // BME280 sensor

void setup_sensors() {
    // Initialize TSL2591 sensor
    if (!tsl.begin()) {
        Serial.println("Could not initialize TSL2591 light sensor!");
    } else {
        Serial.println("TSL2591 initialized successfully.");
    }

    // Initialize INA219 sensors
    if (!ina219Primary.begin()) {
        Serial.println("Primary INA219: Initialization failed, check connections!");
    } else {
        ina219Primary.linearCalibrate(ina219Reading_mA, extMeterReading_mA);
        Serial.println("Primary INA219 initialized successfully.");
    }

    if (!ina219Secondary.begin()) {
        Serial.println("Secondary INA219: Initialization failed, check connections!");
    } else {
        Serial.println("Secondary INA219 initialized successfully.");
    }

    // Initialize BME280 sensor
    if (!bme.begin(0x76)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
    } else {
        Serial.println("BME280 initialized successfully.");
    }

    // Initialize mutex for thread-safe operations
    tslMutex = xSemaphoreCreateMutex();
}

void publishSensorData(const char* topic, const String& message) {
    if (!mqttClient.connected()) {
        Serial.println("MQTT not connected. Attempting to reconnect...");
        mqtt_reconnect();
    }
    if (mqttClient.connected()) {
        mqttClient.publish(topic, message.c_str());
    } else {
        Serial.println("Failed to connect to MQTT.");
    }
}

void readVoltageSupply() {
    float voltage_V = ina219Secondary.getBusVoltage_V();
    String voltageMessage = "{\"value\": " + String(voltage_V) + "}";
    publishSensorData(topic_voltage_supply, voltageMessage);
}

void readPowerConsumption() {
    float voltage_V = ina219Primary.getBusVoltage_V();
    float current_mA = ina219Primary.getCurrent_mA();
    float power_mW = ina219Primary.getPower_mW();
    String voltageMessage = "{\"value\": " + String(voltage_V) + "}";
    String currentMessage = "{\"value\": " + String(current_mA) + "}";
    String powerMessage = "{\"value\": " + String(power_mW) + "}";
    publishSensorData(topic_voltage_bus, voltageMessage);
    publishSensorData(topic_current, currentMessage);
    publishSensorData(topic_power, powerMessage);
}

void readLightIntensity() {
    if (xSemaphoreTake(tslMutex, portMAX_DELAY)) {
        uint16_t broadband = tsl.getLuminosity(TSL2591_FULLSPECTRUM);
        uint16_t infrared = tsl.getLuminosity(TSL2591_INFRARED);
        uint16_t visible = broadband - infrared;
        xSemaphoreGive(tslMutex);
        String lightMessage = "{\"value\": " + String(visible) + "}";
        publishSensorData(mqtt_topic_light, lightMessage);
    } else {
        Serial.println("Failed to acquire TSL2591 mutex in readLightIntensity.");
    }
}

void readBME280Data() {
    float temperature = bme.readTemperature();
    float humidity = bme.readHumidity();
    float pressure = bme.readPressure() / 100.0F;  // Convert Pa to hPa

    // Use String formatting to avoid quotes around the float values
    String temperatureMessage = "{\"value\": " + String(temperature, 2) + "}"; // Two decimal places
    String humidityMessage = "{\"value\": " + String(humidity, 2) + "}"; // Two decimal places
    String pressureMessage = "{\"value\": " + String(pressure, 2) + "}"; // Two decimal places

    publishSensorData(topic_temperature, temperatureMessage);
    publishSensorData(topic_humidity, humidityMessage);
    publishSensorData(topic_pressure, pressureMessage);
}

void taskReadSensors(void *parameter) {
    while (true) {
        readLightIntensity();
        readPowerConsumption();
        readVoltageSupply();
        readBME280Data();
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}
