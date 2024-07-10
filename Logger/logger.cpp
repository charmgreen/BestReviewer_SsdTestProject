#pragma once
#include <iostream>
#include <ctime>

class Logger {
public:
    static void print(const char* funcName, const char* logMessage) {
        time_t timer = time(NULL);
        tm nowTime;
        localtime_s(&nowTime, &timer);

        printf("[%04d.%02d.%02d %02d:%02d:%02d] %s() : %s\n", 1900 + nowTime.tm_year,
            nowTime.tm_mon + 1, nowTime.tm_mday, nowTime.tm_hour,
            nowTime.tm_min, nowTime.tm_sec, funcName, logMessage);
    }
};