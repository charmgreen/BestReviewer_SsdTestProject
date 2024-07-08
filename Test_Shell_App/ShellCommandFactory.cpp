// Copyright [2024] <CRA/BestReviewer>

#include "ShellCommandFactory.h"

ShellCommand* ShellCommandFactory::Parse(const std::string& strCommand) {
    TokenArgument(strCommand);
    MakeCommand();
    result->eCommand = GetCmdType();
    result->LBA = GetLBA();
    result->Data = GetData();
    return result;
}

void ShellCommandFactory::TokenArgument(const std::string& strCommand) {
    std::string token;
    size_t start = 0, end = 0;

    while ((end = strCommand.find(' ', start)) != std::string::npos) {
        token = strCommand.substr(start, end - start);
        CommandToken.push_back(token);
        start = end + 1;
    }

    // 마지막 단어 처리
    token = strCommand.substr(start);
    CommandToken.push_back(token);
}

void ShellCommandFactory::MakeCommand() {
    result = new ShellCommand();
}


ShellCmdType ShellCommandFactory::GetCmdType() {
    ShellCmdType eCmdType = ShellCmdType::Invalid;

    if (CommandToken[0] == "write") eCmdType = ShellCmdType::Write;
    if (CommandToken[0] == "read") eCmdType = ShellCmdType::Read;
    if (CommandToken[0] == "exit") eCmdType = ShellCmdType::Exit;
    if (CommandToken[0] == "help") eCmdType = ShellCmdType::Help;
    if (CommandToken[0] == "fullwrite") eCmdType = ShellCmdType::FullWrite;
    if (CommandToken[0] == "fullread") eCmdType = ShellCmdType::FullRead;
    if (CommandToken[0] == "testapp1") eCmdType = ShellCmdType::TestApp1;
    if (CommandToken[0] == "testapp2") eCmdType = ShellCmdType::TestApp2;

    return eCmdType;
}

int ShellCommandFactory::GetLBA() {
    if ((result->eCommand == ShellCmdType::Write) ||
        (result->eCommand == ShellCmdType::Read)) {
        return std::stoi(CommandToken[1]);
    }
    return 0;
}

unsigned int ShellCommandFactory::GetData() {
    if (result->eCommand == ShellCmdType::Write) {
        return std::stoul(CommandToken[2], nullptr, 16);
    } else if (result->eCommand == ShellCmdType::FullWrite) {
        return std::stoul(CommandToken[1], nullptr, 16);
    } else {
        return 0;
    }
}

