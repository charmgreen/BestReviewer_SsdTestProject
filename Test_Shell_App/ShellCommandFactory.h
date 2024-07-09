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
    TestApp1,
    TestApp2,
    Invalid,
};

class ShellCommand {
 public:
    ShellCmdType eCommand;
    std::string strLBA;
    std::string strData;
    int LBA;
    bool IsInvalid;
};

class ShellCommandFactory {
 public:
    ShellCommand* Parse(const std::string& strCommand);
 private:
    void TokenArgument(const std::string& strCommand);
    void MakeCommand();
    void GetCmdType();
    void GetLBA();
    void GetData();

    std::vector<std::string> CommandToken;
    ShellCommand* result;
};