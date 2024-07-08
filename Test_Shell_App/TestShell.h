#pragma once
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include "SsdDriver.h"


class TestShell {
 public:
    bool GetUserInput(char str[]);
    void Run();
    void SetSsdDriver(SsdDriver* ssddriver);

 private:
    void parseArgument();
    static const int MAX_INPUT_LENGTH = 1000;
    std::vector<std::string> RawInput;

    enum class ShellCommand {
        Write,
        Read,
    };

    SsdDriver* ssddriver;
};
