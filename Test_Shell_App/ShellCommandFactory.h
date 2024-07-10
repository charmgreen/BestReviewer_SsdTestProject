// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <vector>
#include <string>
#include "ShellCommand.h"

class ShellCommandFactory {
 public:
    ShellCommand* Make(const std::string& strCommand);
 private:
    void TokenArgument(const std::string& strCommand);
    void MakeCommand();
    ShellCommand* MakeInvalidCommand();
    ShellCommand* MakeWriteCommand();
    ShellCommand* MakeReadCommand();
    ShellCommand* MakeExitCommand();
    ShellCommand* MakeHelpCommand();
    ShellCommand* MakeFullWriteCommand();
    ShellCommand* MakeFullReadCommand();
    ShellCommand* MakeTestApp1Command();
    ShellCommand* MakeTestApp2Command();

    bool IsStringDecimal(const std::string& str);
    bool IsStringHexadecimal(const std::string& str);
    bool IsStringValidLBA(const std::string& str);

    std::vector<std::string> CommandToken;
    ShellCommand* result;

    const int MAX_STR_LENGTH_DATA = 10;
    const int MIN_LBA = 0;
    const int MAX_LBA = 99;
};
