// Copyright [2024] <CRA/BestReviewer>

#include "ShellCommandFactory.h"

ShellCommand ShellCommandFactory::Parse(const std::string& strCommand) {
    TokenArgument(strCommand);

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

void ShellCommandFactory::GetCmdType() {
}

void ShellCommandFactory::GetLBA() {
}

void ShellCommandFactory::GetData() {
}

