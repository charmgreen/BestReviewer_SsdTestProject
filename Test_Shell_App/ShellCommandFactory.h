// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <vector>
#include <string>

enum class ShellCmdType {
    Write,
    Read,
    Exit,
    Help,
    FullWrite,
    FullRead,
};

class ShellCommand {
    ShellCmdType eCommand;
    int LBA;
    int Data;
};

class ShellCommandFactory {
 public:
    ShellCommand Parse(const std::string& strCommand);
 private:
    void TokenArgument(const std::string& strCommand);
    void GetCmdType();
    void GetLBA();
    void GetData();

    std::vector<std::string> CommandToken;
    ShellCommand result;
};
