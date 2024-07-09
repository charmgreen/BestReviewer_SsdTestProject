// Copyright [2024] <CRA/BestReviewer>

#include "ShellCommandFactory.h"
#include "ShellScript.h"

ShellCommand* ShellCommandFactory::Make(const std::string& strCommand) {
    TokenArgument(strCommand);
    MakeCommand();
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
    if (CommandToken.empty() == true) result = MakeInvalidCommand();
    else if (CommandToken[0] == "write") result = MakeWriteCommand();
    else if (CommandToken[0] == "read") result = MakeReadCommand();
    else if (CommandToken[0] == "exit") result = MakeExitCommand();
    else if (CommandToken[0] == "help") result = MakeHelpCommand();
    else if (CommandToken[0] == "fullwrite") result = MakeFullWriteCommand();
    else if (CommandToken[0] == "fullread") result = MakeFullReadCommand();
    else if (CommandToken[0] == "testapp1") result = MakeTestApp1Command();
    else if (CommandToken[0] == "testapp2") result = MakeTestApp2Command();
    else
        result = MakeInvalidCommand();
}

ShellCommand* ShellCommandFactory::MakeInvalidCommand() {
    return new InvalidCommand();
}

ShellCommand* ShellCommandFactory::MakeWriteCommand() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 3) {
        return new InvalidCommand();
    }

    // Check Invalid 2) LBA
    for (char ch = 0; ch < CommandToken[1].size(); ch++) {
        if ('0' > CommandToken[1][ch] || CommandToken[1][ch] > '9') {
            return new InvalidCommand();
        }
    }

    if (std::stoi(CommandToken[1]) >= 100) {
        return new InvalidCommand();
    }

    // Check Invalid 3) Data
    std::string strData = CommandToken[2];
    if (CommandToken[2].empty() == true) {
        return new InvalidCommand();
    }

    if ((strData[0] != '0') ||
        (strData[1] != 'x') ||
        (strData.size() != 10)) {
        return new InvalidCommand();
    }
    for (int idx = 2; idx < strData.size(); idx++) {
        if (('0' > strData[idx] || strData[idx] > '9') &&
            ('A' > strData[idx] || strData[idx] > 'F')) {
            return new InvalidCommand();
        }
    }

    return new WriteCommand(CommandToken[1], strData);
}


ShellCommand* ShellCommandFactory::MakeReadCommand() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 2) {
        return new InvalidCommand();
    }

    // Check Invalid 2) LBA
    for (char ch = 0; ch < CommandToken[1].size(); ch++) {
        if ('0' > CommandToken[1][ch] || CommandToken[1][ch] > '9') {
            return new InvalidCommand();
        }
    }

    if (std::stoi(CommandToken[1]) >= 100) {
        return new InvalidCommand();
    }

    return new ReadCommand(CommandToken[1]);
}

ShellCommand* ShellCommandFactory::MakeExitCommand() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 1) {
        return new InvalidCommand();
    }

    return new ExitCommand();
}

ShellCommand* ShellCommandFactory::MakeHelpCommand() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 1) {
        return new InvalidCommand();
    }

    return new HelpCommand();
}

ShellCommand* ShellCommandFactory::MakeFullWriteCommand() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 2) {
        return new InvalidCommand();
    }

    // Check Invalid 2) Data
    std::string strData = CommandToken[1];
    if (CommandToken[1].empty() == true) {
        return new InvalidCommand();
    }

    if ((strData[0] != '0') ||
        (strData[1] != 'x') ||
        (strData.size() != 10)) {
        return new InvalidCommand();
    }
    for (int idx = 2; idx < strData.size(); idx++) {
        if (('0' > strData[idx] || strData[idx] > '9') &&
            ('A' > strData[idx] || strData[idx] > 'F')) {
            return new InvalidCommand();
        }
    }

    return new FullWriteCommand(strData);
}

ShellCommand* ShellCommandFactory::MakeFullReadCommand() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 1) {
        return new InvalidCommand();
    }

    return new FullReadCommand();
}

ShellCommand* ShellCommandFactory::MakeTestApp1Command() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 1) {
        return new InvalidCommand();
    }

    return new ShellScript("testapp1.txt");
}

ShellCommand* ShellCommandFactory::MakeTestApp2Command() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 1) {
        return new InvalidCommand();
    }

    return new ShellScript("testapp2.txt");
}
