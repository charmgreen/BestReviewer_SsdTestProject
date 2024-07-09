// Copyright [2024] <CRA/BestReviewer>

#include "ShellCommandFactory.h"

ShellCommand* ShellCommandFactory::Parse(const std::string& strCommand) {
    TokenArgument(strCommand);
    MakeCommand();
    GetCmdType();
    GetLBA();
    GetData();
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


void ShellCommandFactory::GetCmdType() {
    if (CommandToken[0] == "write") result->eCommand = ShellCmdType::Write;
    else if (CommandToken[0] == "read") result->eCommand = ShellCmdType::Read;
    else if (CommandToken[0] == "exit") result->eCommand = ShellCmdType::Exit;
    else if (CommandToken[0] == "help") result->eCommand = ShellCmdType::Help;
    else if (CommandToken[0] == "fullwrite") result->eCommand = ShellCmdType::FullWrite;
    else if (CommandToken[0] == "fullread") result->eCommand = ShellCmdType::FullRead;
    else if (CommandToken[0] == "testapp1") result->eCommand = ShellCmdType::TestApp1;
    else if (CommandToken[0] == "testapp2") result->eCommand = ShellCmdType::TestApp2;
    else {
        result->eCommand = ShellCmdType::Invalid;
        result->IsInvalid = true;
    }
}

void ShellCommandFactory::GetLBA() {
    if ((result->eCommand == ShellCmdType::Write) ||
        (result->eCommand == ShellCmdType::Read)) {

        if (CommandToken[1].empty() == true)
        {
            result->IsInvalid = true;
            return;
        }

        for (char ch = 0; ch < CommandToken[1].size(); ch++)
        {
            if ('0' > CommandToken[1][ch] || CommandToken[1][ch] > '9') {
                result->IsInvalid = true;
                return;
            }
        }
        result->strLBA = CommandToken[1];
        result->LBA = std::stoi(CommandToken[1]);

        if (result->LBA >= 100) result->IsInvalid = true;
    }
    else if (result->eCommand == ShellCmdType::FullWrite)
    {
        // Do Nothing
    }
    else
    {
        if (CommandToken.size() > 1) {
            result->IsInvalid = true;
        }
    }
}

void ShellCommandFactory::GetData() {
    std::string strData;
    if (result->eCommand == ShellCmdType::Write) {
        strData = CommandToken[2];
        if (strData.empty() == true)
        {
            result->IsInvalid = true;
            return;
        }
    } else if (result->eCommand == ShellCmdType::FullWrite) {
        strData = CommandToken[1];
        if (strData.empty() == true)
        {
            result->IsInvalid = true;
            return;
        }
    } else {
        if (CommandToken.size() > 2)
        {
            result->IsInvalid = true;
        }
        return;
    }

    if ((strData[0] != '0') ||
        (strData[1] != 'x') ||
        (strData.size() != 10)) {
        result->IsInvalid = true;
        return;
    }
    for (int idx = 2; idx < strData.size(); idx++) {
        if (('0' > strData[idx] || strData[idx] > '9') &&
            ('A' > strData[idx] || strData[idx] > 'F')) {
            result->IsInvalid = true;
            return;
        }
    }       

    result->strData = strData;
}

