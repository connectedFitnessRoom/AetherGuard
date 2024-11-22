/* ----------------- SENSORS ----------------- 
DHT11–Temperature and Humidity Sensor

  - install the library : DHT sensor library

  - https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf?srsltid=AfmBOorOwhZeWdtxGOkinRcoDmVsUm-OgDXiS5XhmuDKHVREJp-Ql8RF (datasheet)
  - https://tutoduino.fr/debuter/capteur-temperature/ (tuto fr)
  - https://www.upesy.fr/blogs/tutorials/dht11-humidity-temperature-sensor-with-arduino-code-on-esp32-board?srsltid=AfmBOoqVhRMwXAssA7x3zVHS4BSU7E0lccARz04wQbHNPpcT92JzGbXO (tuto fr)
  - On the sensor I tested, the resistors blocked the data flow. Even if online they recommended 4.7k to 10k resistance. I assume my sensor has built-in resistance

MQ-135 AirQuality Sensor

  - You NEED to change the private variables of the MQ135.h to public in order to calibrate automaticaly the sensor.
  - install the library MQT135

  - https://www.olimex.com/Products/Components/Sensors/Gas/SNS-MQ135/resources/SNS-MQ135.pdf (datasheet)
  - https://github.com/Cha0s0000/Utopian/blob/master/%5BArduino%20basics%20tutorials%5D%20use%20MQ135%20air%20quality%20detecting%20module.md (tuto)
  - https://github.com/mdsiraj1992/Gassensors/blob/master/arduino%20codes/mq135_sensor/mq135_sensor.ino (tuto/code)
  - https://www.codrey.com/electronic-circuits/how-to-use-mq-135-gas-sensor/ (info about ppm quality (used in work parameters))
  - the sensor needs to be calibrated before use
  - If I understand correctly, the digital output gives us a true false indicator of air quality, while the analog gives us more details on the different components of the air(
ammonia, benzene, alcohols, nitrogen oxide, carbon monoxide)
  - 10k resistor on data outputs and 3.3v

*/

//For the sensors to work correctly and send the right values, you sometimes have to let them run for several hours! Because a sensor can retain residues linked to its manufacturing, which distorts the values ​​in the first hours.
// https://www.upesy.fr/blogs/tutorials/get-date-time-on-esp32-with-ntp-server-arduino-code?srsltid=AfmBOooBuKJPgIIyyKEwJA_6kN22pXcP2x9Fr4WWSt30wV8Qs-9uzLey (tuto date)



// ESP8266



// ------------ INCLUDES ------------
#include <DHT.h>             // Library for the DHT sensor
#include <MQ135.h>           // Library for the MQ135 sensor
#include <ESP8266WiFi.h>     // Library for WiFi connection
#include <WiFiUdp.h>         // UDP library for NTP
#include <NTPClient.h>       // Library to fetch time from an NTP server

// ------------ PINS ------------
#define DHTPIN 14            // GPIO14 (D5 on ESP8266) for DHT11 sensor
#define DHTTYPE DHT11        // Sensor type DHT11
#define MQ135_ANALOG_PIN A0  // A0 for MQ-135 analog output

// ------------ SENSORS ------------
DHT dhtSensor(DHTPIN, DHTTYPE);
MQ135 MQ135Sensor(MQ135_ANALOG_PIN);

// ------------ PARAMETERS ------------
#define WAITTIME 5000             // Delay of 5 seconds between measurements
#define CAUTION_THRESHOLD 750     // Medium-level air quality caution threshold
#define DANGEROUS_THRESHOLD 1200  // High-level dangerous air quality threshold

// ------------ CALIBRATION ------------
#define CALIBRATION_HOUR 8        // Calibration time (hour)
#define CALIBRATION_MINUTE 0      // Calibration time (minute)

// ------------ WIFI CONFIGURATION ------------
const char* ssid = "your_ssid";               // WiFi SSID
const char* password = "your_password";       // WiFi password

// ------------ NTP TIME CONFIGURATION ------------
const char* ntpServer = "pool.ntp.org";       // NTP server address
const long  gmtOffset_sec = 3600;             // GMT offset in seconds (UTC+1 for Belgium)
const int   daylightOffset_sec = 3600;        // Offset for daylight saving time (1 hour)

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec);



void setup() {
  Serial.begin(9600);

  dhtSensor.begin();
  MQ135Sensor._rzero = MQ135Sensor.getRZero();

  wifiConnection();
}

void loop() {
  
  timeClient.update(); // Update time from NTP server
  if (timeClient.getEpochTime() > 0) {  // Check if the time has been updated successfully
    Serial.println("Heure actuelle : ");
    Serial.println(timeClient.getFormattedTime());
  } else {
    Serial.println("ERROR - Time");
  }


  // Calibrate sensors
  calibrateAtSpecificTime();

  // Read and display values for MQ135 (AirQuality)
  readAndDisplayMQ135Values();

  // Read and display values for DHT11 (Temperature and Humidity)
  readAndDisplayDHTValues();
  

  // Wait before the next reading
  delay(WAITTIME);
}


void wifiConnection() {
  // code inspired by the tutorial above
  WiFi.begin(ssid, password);
  Serial.println("\n Connexion wifi en cours");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\n Connecion wifi réussie");
  Serial.print("Local ESP8266 IP: ");
  Serial.println(WiFi.localIP());

  // NTP server configuration
  timeClient.begin();
}

void calibrateAtSpecificTime() {
  // Get the current time from the NTP client (epoch time in seconds)
  unsigned long currentEpochTime = timeClient.getEpochTime() % 86400; // 86400 sec in one day
  unsigned long targetEpochTime = (CALIBRATION_HOUR * 3600) + (CALIBRATION_MINUTE * 60); // In seconds since midnight


  // Check if the current time has passed the target calibration time
  if (currentEpochTime >= targetEpochTime and currentEpochTime < targetEpochTime + WAITTIME) {
    MQ135Sensor._rzero = MQ135Sensor.getRZero();
    Serial.print("MQT135 - Calibration RZero : ");
    Serial.println(MQ135Sensor._rzero);
  }
}


void readAndDisplayDHTValues() {
  float temperature = dhtSensor.readTemperature();
  float humidity = dhtSensor.readHumidity();

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
}

void readAndDisplayMQ135Values() {
  float ppm = MQ135Sensor.getPPM();
  Serial.print("Particules de gaz détecté (pour 1 000 000 de particules d'air) = ");
  Serial.print(ppm);

  if (ppm>DANGEROUS_THRESHOLD) {
    Serial.println(" Qualité de l'air dangereuse");
  } else if (ppm>CAUTION_THRESHOLD) {
    Serial.println(" Qualité de l'air pas bonne");
  } else {
    Serial.println(" Qualité de l'air bonne");
  }
}