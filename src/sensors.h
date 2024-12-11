#ifndef SENSORS_H
#define SENSORS_H


#include <Adafruit_TSL2591.h>
#include <DFRobot_INA219.h>

extern Adafruit_TSL2591 tsl;


void setup_sensors();
void readPowerConsumption();
void readLightIntensity();
void readBME280Data();
void taskReadSensors(void *parameter);

#endif
