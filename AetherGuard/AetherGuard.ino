/* ----------------- SENSORS ----------------- 
DHT11–Temperature and Humidity Sensor

  - install the library : DHT sensor library

  - https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf?srsltid=AfmBOorOwhZeWdtxGOkinRcoDmVsUm-OgDXiS5XhmuDKHVREJp-Ql8RF (datasheet)
  - https://tutoduino.fr/debuter/capteur-temperature/ (tuto fr)
  - https://www.upesy.fr/blogs/tutorials/dht11-humidity-temperature-sensor-with-arduino-code-on-esp32-board?srsltid=AfmBOoqVhRMwXAssA7x3zVHS4BSU7E0lccARz04wQbHNPpcT92JzGbXO (tuto fr)
  - On the sensor I tested, the resistors blocked the data flow. Even if online they recommended 4.7k to 10k resistance. I assume my sensor has built-in resistance

MQ-135 AirQuality Sensor

  - install the library MQT135
  - You NEED to change the private variables of the MQ135.h to public in order to calibrate automaticaly the sensor.

  - https://www.olimex.com/Products/Components/Sensors/Gas/SNS-MQ135/resources/SNS-MQ135.pdf (datasheet)
  - https://github.com/Cha0s0000/Utopian/blob/master/%5BArduino%20basics%20tutorials%5D%20use%20MQ135%20air%20quality%20detecting%20module.md (tuto)
  - https://github.com/mdsiraj1992/Gassensors/blob/master/arduino%20codes/mq135_sensor/mq135_sensor.ino (tuto/code)
  - https://www.codrey.com/electronic-circuits/how-to-use-mq-135-gas-sensor/ (info about ppm quality (used in work parameters))
  - the sensor needs to be calibrated before use
  - If I understand correctly, the digital output gives us a true false indicator of air quality, while the analog gives us more details on the different components of the air(
ammonia, benzene, alcohols, nitrogen oxide, carbon monoxide)
  - 10k resistor on data outputs and 3.3v

0,96-zool oled display 

  -  https://www.mouser.com/datasheet/2/1398/Soldered_333099-3395096.pdf?srsltid=AfmBOooTmRiDG2rQgAWGAja9TuC1vTCoJ541tHopa_ZUtx9-k8u0TNcs (datasheet)

*/

// Include headers
#include "config.h"
#include "wifi_mqtt.h"
#include "time_utils.h"
#include "sensors.h"
#include "utils.h"

void setup() {
    Serial.begin(9600);
    initSensors();
    connectToWiFi();
    connectToMQTT();
}

void loop() {
    String currentTime = getFormattedDateTime();

    calibrateAtSpecificTime();
    float temperature = readTemperature();
    float humidity = readHumidity();
    float ppm = readAirQuality();

    Serial.print("Date et heure actuelles : ");
    Serial.println(currentTime);
    displaySensorValues(temperature, humidity, ppm);
    publishSensorData(temperature, humidity, ppm, currentTime);

    delay(WAITTIME);
}
