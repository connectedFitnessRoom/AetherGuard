/* ----------------- SENSORS ----------------- 
DHT11–Temperature and Humidity Sensor
  - install the library : DHT sensor library
  - https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf?srsltid=AfmBOorOwhZeWdtxGOkinRcoDmVsUm-OgDXiS5XhmuDKHVREJp-Ql8RF (datasheet)
  - https://tutoduino.fr/debuter/capteur-temperature/ (tuto fr)
  - https://www.upesy.fr/blogs/tutorials/dht11-humidity-temperature-sensor-with-arduino-code-on-esp32-board?srsltid=AfmBOoqVhRMwXAssA7x3zVHS4BSU7E0lccARz04wQbHNPpcT92JzGbXO (tuto fr)
  - On the sensor I tested, the resistors blocked the data flow. Even if online they recommended 4.7k to 10k resistance. I assume my sensor has built-in resistance

*/

// ESP8266


#include "DHT.h"

#define DHTPIN 4  // Using GPIO4 (D2 on the ESP8266)
#define DHTTYPE DHT11 // Set sensor type DHT11
#define WAITTIME 5000 // 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (isnan(temperature)) {
    Serial.println("DHT11 ERROR - Temperature");
  } else {
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println(" °C");
  }
  if (isnan(humidity)) {
    Serial.println("DHT11 ERROR - Temperature");
  } else {
    Serial.print("Humidity = ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  delay(5000);
}