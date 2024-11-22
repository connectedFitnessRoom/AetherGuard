/* ----------------- SENSORS ----------------- 
DHT11–Temperature and Humidity Sensor
  - install the library : DHT sensor library
  - https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf?srsltid=AfmBOorOwhZeWdtxGOkinRcoDmVsUm-OgDXiS5XhmuDKHVREJp-Ql8RF (datasheet)
  - https://tutoduino.fr/debuter/capteur-temperature/ (tuto fr)
  - https://www.upesy.fr/blogs/tutorials/dht11-humidity-temperature-sensor-with-arduino-code-on-esp32-board?srsltid=AfmBOoqVhRMwXAssA7x3zVHS4BSU7E0lccARz04wQbHNPpcT92JzGbXO (tuto fr)
  - On the sensor I tested, the resistors blocked the data flow. Even if online they recommended 4.7k to 10k resistance. I assume my sensor has built-in resistance
MQ-135 AirQuality Sensor
  - install the library MQT135
  - https://www.olimex.com/Products/Components/Sensors/Gas/SNS-MQ135/resources/SNS-MQ135.pdf (datasheet)
  - https://github.com/Cha0s0000/Utopian/blob/master/%5BArduino%20basics%20tutorials%5D%20use%20MQ135%20air%20quality%20detecting%20module.md (tuto)
  - https://www.codrey.com/electronic-circuits/how-to-use-mq-135-gas-sensor/ (info about ppm quality (used in work parameters))
  - the sensor needs to be calibrated before use
  - If I understand correctly, the digital output gives us a true false indicator of air quality, while the analog gives us more details on the different components of the air(
ammonia, benzene, alcohols, nitrogen oxide, carbon monoxide)
  - 10k resistor on data outputs and 3.3v
*/

//For the sensors to work correctly and send the right values, you sometimes have to let them run for several hours! Because a sensor can retain residues linked to its manufacturing, which distorts the values ​​in the first hours.

// ESP8266

#include "DHT.h"
#include "MQ135.h"

/* PINS */
#define DHTPIN 14     // Using GPIO14 (D5 on the ESP8266)
#define DHTTYPE DHT11 // Set sensor type DHT11
#define MQ135_ANALOG_PIN A0     // Using A0 for analog output of MQ-135

/* Work parameters */
#define WAITTIME 5000 // 5 seconds delay between measurements
#define CAUTION_THRESHOLD 750    // Mid-level caution range starts here
#define DANGEROUS_THRESHOLD 1200 // Dangerously high levels


DHT dhtSensor(DHTPIN, DHTTYPE);
MQ135 MQ135Sensor(MQ135_ANALOG_PIN);

void setup() {
  Serial.begin(9600);
  dhtSensor.begin();
}

void loop() {
  // Read temperature and humidity from DHT11
  float temperature = dhtSensor.readTemperature();
  float humidity = dhtSensor.readHumidity();
  
  // Check for DHT11 errors and display readings
  if (isnan(temperature)) {
    Serial.println("DHT11 ERROR - Temperature");
  } else {
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println(" °C");
  }
  
  if (isnan(humidity)) {
    Serial.println("DHT11 ERROR - Humidity");
  } else {
    Serial.print("Humidity = ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  // Read analog value MQ135
  //float rzero = MQ135Sensor.getRZero(); //calibration: to be done before installing (see the tuto)
  //Serial.println(rzero);
  float ppm = MQ135Sensor.getPPM();
  Serial.println("Particules de gaz détecté (pour 1 000 000 de particules d'air) = ");
  Serial.println(ppm);
  if (ppm>DANGEROUS_THRESHOLD) {
    Serial.println("Qualité de l'air dangereuse");
  } else if (ppm>CAUTION_THRESHOLD) {
    Serial.println("Qualité de l'air pas bonne");
  } else {
    Serial.println("Qualité de l'air bonne");
  }
  

  // Wait before the next reading
  delay(WAITTIME);
}