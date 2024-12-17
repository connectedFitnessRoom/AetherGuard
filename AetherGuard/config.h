#ifndef CONFIG_H
#define CONFIG_H

// WiFi configuration
const char* ssid = "FitnessClub";
const char* password = "Fitness123";
//const char* ssid = "Corentin's Galaxy A54 5G";
//const char* password = "7777abcd";

// MQTT configuration
const char* MQTT_BROKER = "192.168.70.39";
const int MQTT_BROKER_PORT = 1883;
const char* MQTT_TOPIC = "AetherGuard/sensordata";

// NTP configuration
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// Sensor pins and types
#define DHTPIN 14
#define DHTTYPE DHT11
#define MQ135_ANALOG_PIN A0

// Air quality thresholds
#define CAUTION_THRESHOLD 750
#define DANGEROUS_THRESHOLD 1200

// Calibration time
#define CALIBRATION_HOUR 8
#define CALIBRATION_MINUTE 0

// Wait time between measurements
#define WAITTIME 5000

#endif