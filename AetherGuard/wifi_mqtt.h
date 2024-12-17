#ifndef WIFI_MQTT_H
#define WIFI_MQTT_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "utils.h"

WiFiClient espClient;
PubSubClient client(espClient);

void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.println("\nConnexion WiFi en cours...");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("\nConnexion WiFi réussie");
    Serial.print("Local ESP8266 IP: ");
    Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  client.setServer(MQTT_BROKER, MQTT_BROKER_PORT);
    if (!client.connected()) {
        Serial.println("Tentative de connexion MQTT...");
        if (client.connect("ESPClient")) {
            Serial.println("MQTT connecté.");
        } else {
            Serial.print("MQTT connection failed. Error: ");
            Serial.println(client.state());
        }
    }
}

void publishSensorData(float temperature, float humidity, float ppm, String currentTime) {
    String payload = createJSONPayload(temperature, humidity, ppm, currentTime);
    if (client.connected()) {
        client.publish(MQTT_TOPIC, payload.c_str());
        Serial.print("Données publiées : ");
        Serial.println(payload);
    } else {
        Serial.println("Erreur MQTT - Client non connecté");
    }
}

#endif