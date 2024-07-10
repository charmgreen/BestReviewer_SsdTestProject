// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <vector>
#include <string>
#include "ShellCommand.h"
#include "SsdDriver.h"

class ShellCommandFactory {
 public:
    ShellCommand* Make(const std::string& strCommand);
    void SetSsdDriver(SsdDriver* ssddriver);
 protected:
    SsdDriver* ssddriver;
 private:
    void TokenArgument(const std::string& strCommand);
    void MakeCommand();
    ShellCommand* MakeInvalidCommand();
    ShellCommand* MakeWriteCommand();
    ShellCommand* MakeReadCommand();
    ShellCommand* MakeEraseCommand();
    ShellCommand* MakeEraseRangeCommand();
    ShellCommand* MakeFlushCommand();
    ShellCommand* MakeExitCommand();
    ShellCommand* MakeHelpCommand();
    ShellCommand* MakeFullWriteCommand();
    ShellCommand* MakeFullReadCommand();
    ShellCommand* MakeTestApp1Command();
    ShellCommand* MakeTestApp2Command();

    bool IsStringDecimal(const std::string& str);
    bool IsStringHexadecimal(const std::string& str);
    bool IsStringValidLBA(const std::string& str);
    bool IsStringValidLength(const std::string& strSize);
    bool IsStringValidLength(const std::string& strStartLBA, const std::string& strEndLBA);
    std::string LimitToMinLBA(const std::string& str);
    std::string LimitToMaxLBA(const std::string& str);

    std::vector<std::string> CommandToken;
    ShellCommand* result;

    const int MAX_STR_LENGTH_DATA = 10;
};
