#ifndef UTILS_H
#define UTILS_H

String createJSONPayload(float temperature, float humidity, float ppm, String timestamp) {
    String json = "{";
    json += "\"temperature\":";
    json += String(temperature, 1);
    json += ",";
    json += "\"humidity\":";
    json += String(humidity, 1);
    json += ",";
    json += "\"ppm\":";
    json += String(ppm, 1);
    json += ",";
    json += "\"date\":\"";
    json += timestamp;
    json += "\"}";
    return json;
}

#endif