#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>
#include "MQ135.h"
#include "config.h"

DHT dhtSensor(DHTPIN, DHTTYPE);
MQ135 MQ135Sensor(MQ135_ANALOG_PIN);

void initSensors() {
    dhtSensor.begin();
    MQ135Sensor._rzero = MQ135Sensor.getRZero();
}

void calibrateAtSpecificTime() {
    time_t currentEpochTime = timeClient.getEpochTime() % 86400;
    time_t targetEpochTime = (CALIBRATION_HOUR * 3600) + (CALIBRATION_MINUTE * 60);

    if (currentEpochTime >= targetEpochTime && currentEpochTime < targetEpochTime + WAITTIME) {
        MQ135Sensor._rzero = MQ135Sensor.getRZero();
        Serial.print("Calibration MQ135 RZero : ");
        Serial.println(MQ135Sensor._rzero);
    }
}

float readTemperature() {
    return dhtSensor.readTemperature();
}

float readHumidity() {
    return dhtSensor.readHumidity();
}

float readAirQuality() {
    return MQ135Sensor.getPPM();
}

void displaySensorValues(float temperature, float humidity, float ppm) {
    Serial.print("Température : ");
    Serial.println(temperature);
    Serial.print("Humidité : ");
    Serial.println(humidity);
    Serial.print("Qualité de l'air (ppm) : ");
    Serial.println(ppm);
}

#endif