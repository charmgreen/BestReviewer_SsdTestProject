// Copyright 2024, Samsung

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
    CommandToken.clear();

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
    if (IsStringDecimal(CommandToken[1]) == false) {
        return new InvalidCommand();
    }

    if (IsStringValidLBA(CommandToken[1]) == false) {
        return new InvalidCommand();
    }

    // Check Invalid 3) Data
    if (IsStringHexadecimal(CommandToken[2]) == false) {
        return new InvalidCommand();
    }

    return new WriteCommand(CommandToken[1], CommandToken[2]);
}


ShellCommand* ShellCommandFactory::MakeReadCommand() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 2) {
        return new InvalidCommand();
    }

    // Check Invalid 2) LBA
    if (IsStringDecimal(CommandToken[1]) == false) {
        return new InvalidCommand();
    }

    if (IsStringValidLBA(CommandToken[1]) == false) {
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
    if (IsStringHexadecimal(CommandToken[1]) == false) {
        return new InvalidCommand();
    }

    return new FullWriteCommand(CommandToken[1]);
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

    return new TestApp1();
}

ShellCommand* ShellCommandFactory::MakeTestApp2Command() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 1) {
        return new InvalidCommand();
    }

    return new TestApp2();
}

bool ShellCommandFactory::IsStringDecimal(const std::string& str)
{
    for (char ch = 0; ch < str.size(); ch++) {
        if ('0' > str[ch] || str[ch] > '9') {
            return false;
        }
    }
    return true;
}

bool ShellCommandFactory::IsStringHexadecimal(const std::string& str)
{
    if ((str[0] != '0') ||
        (str[1] != 'x') ||
        (str.size() != MAX_STR_LENGTH_DATA)) {
        return false;
    }

    for (int idx = 2; idx < str.size(); idx++) {
        if (('0' > str[idx] || str[idx] > '9') &&
            ('A' > str[idx] || str[idx] > 'F')) {
            return false;
        }
    }
    return true;
}

bool ShellCommandFactory::IsStringValidLBA(const std::string& str)
{
    int LBA = std::stoi(CommandToken[1]);
    if ((MIN_LBA <= LBA) && (LBA <= MAX_LBA)) {
        return true;
    }
    return false;
}
