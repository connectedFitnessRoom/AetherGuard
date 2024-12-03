#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <NTPClient.h>
#include <WiFiUdp.h>
#include "config.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec);

String getFormattedDateTime() {
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    if (epochTime > 0) {
        struct tm* timeInfo = localtime(&epochTime);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
        return String(buffer);
    }
    return "1970-01-01 00:00:00";
}

#endif