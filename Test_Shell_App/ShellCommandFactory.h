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

    std::vector<std::string> CommandToken;
    ShellCommand* result;
};
