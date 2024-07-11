// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>

#define LOG_PRINT(text)  Logger::getInstance().print(__FUNCTION__, text)

class Logger {
 public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
    void print(const char* funcName, const char* logMessage) {
        const unsigned int KB = 1000;
        const unsigned int MAX_LOG_FILE_SIZE = 10 * KB;
        static std::string previousLogFile = "";

        time_t timer = time(NULL);
        tm nowTime;
        localtime_s(&nowTime, &timer);

        const unsigned int BUFFER_SIZE = 200;
        char logCommit[BUFFER_SIZE];
        sprintf_s(logCommit, BUFFER_SIZE,
            "[%02d.%02d.%02d %02d:%02d:%02d] %-5s : %s\n",
            nowTime.tm_year - 100, nowTime.tm_mon + 1, nowTime.tm_mday,
            nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec,
            funcName, logMessage);

        try {
            if (GetLogFileSize(_LOG_FILE) < MAX_LOG_FILE_SIZE) {
                WriteToLogFile(_LOG_FILE, std::ios::app, logCommit);  // append
                return;
            }

            CompressOldestFile(previousLogFile);

            previousLogFile = ChangeNameWithTimeStamp(nowTime);

            WriteToLogFile(_LOG_FILE, std::ios::out, logCommit);  // overwrite
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

 private:
    const char* _LOG_FILE;
    Logger() {
        _LOG_FILE = "latest.log";
        std::ofstream file(_LOG_FILE);
    }

    int GetLogFileSize(const char* fileName) {
        std::ifstream readFile(fileName);
        if (!readFile.is_open()) {
            throw std::runtime_error("[Error] failed to open readFile");
        }
        readFile.seekg(0, std::ios::end);
        int readFileSize = readFile.tellg();

        return readFileSize;
    }

    void WriteToLogFile(const char* fileName,
        std::ios_base::openmode mode,
        const std::string& logContents) {
        std::ofstream writeFile(fileName, mode);
        if (!writeFile.is_open()) {
            throw std::runtime_error("[Error] failed to open writeFile");
        }
        writeFile << logContents;
    }

    void CompressOldestFile(const std::string& oldfile) {
        if (!oldfile.empty()) {
            std::string newFileName =
                oldfile.substr(0, oldfile.find(".")) + std::string(".zip");
            int ret = rename(oldfile.c_str(), newFileName.c_str());
        }
    }

    std::string ChangeNameWithTimeStamp(const tm& nowTime) {
        const unsigned int BUFFER_SIZE = 50;
        char over10KBLogFile[BUFFER_SIZE];
        sprintf_s(over10KBLogFile, BUFFER_SIZE,
            "util_%02d%02d%02d_%02dh_%02dm_%02ds.log",
            nowTime.tm_year - 100, nowTime.tm_mon + 1, nowTime.tm_mday,
            nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
        int ret = rename(_LOG_FILE, over10KBLogFile);
        return over10KBLogFile;
    }
};
