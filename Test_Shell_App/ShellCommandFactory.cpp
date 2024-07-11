// Copyright 2024, Samsung

#include "ShellCommandFactory.h"
#include "../Logger/logger.cpp"

ShellCommand* ShellCommandFactory::Make(const std::string& strCommand) {
    TokenArgument(strCommand);
    MakeCommand();
    return result;
}

void ShellCommandFactory::SetSsdDriver(SsdDriver* ssddriver) {
    this->ssddriver = ssddriver;
}

void ShellCommandFactory::TokenArgument(const std::string& strCommand) {
    Logger::getInstance().print(__FUNCTION__, "Separate commands into tokens");
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
    Logger::getInstance().print(__FUNCTION__, "Generate the appropriate command");
    if (CommandToken.empty() == true) result = MakeInvalidCommand();
    else if (CommandToken[0] == "write") result = MakeWriteCommand();
    else if (CommandToken[0] == "read") result = MakeReadCommand();
    else if (CommandToken[0] == "erase") result = MakeEraseCommand();
    else if (CommandToken[0] == "erase_range") result = MakeEraseRangeCommand();
    else if (CommandToken[0] == "flush") result = MakeFlushCommand();
    else if (CommandToken[0] == "exit") result = MakeExitCommand();
    else if (CommandToken[0] == "help") result = MakeHelpCommand();
    else if (CommandToken[0] == "fullwrite") result = MakeFullWriteCommand();
    else if (CommandToken[0] == "fullread") result = MakeFullReadCommand();
    else if (CommandToken[0] == "testapp1") result = MakeTestApp1Command();
    else if (CommandToken[0] == "testapp2") result = MakeTestApp2Command();
    else if (CommandToken[0] == "compare") result = MakeCompareCommand();
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

ShellCommand* ShellCommandFactory::MakeEraseCommand() {
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

    // Check Invalid 3) Size
    if (IsStringDecimal(CommandToken[2]) == false) {
        return new InvalidCommand();
    }

    if (IsStringValidLength(CommandToken[2]) == false) {
        return new InvalidCommand();
    }

    return new EraseCommand(CommandToken[1], CommandToken[2]);
}

ShellCommand* ShellCommandFactory::MakeEraseRangeCommand() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 3) {
        return new InvalidCommand();
    }

    // Check Invalid 2) StartLBA
    if (IsStringDecimal(CommandToken[1]) == false) {
        return new InvalidCommand();
    }

    if (IsStringValidLBA(CommandToken[1]) == false) {
        return new InvalidCommand();
    }

    // Check Invalid 3) EndLBA
    if (IsStringDecimal(CommandToken[2]) == false) {
        return new InvalidCommand();
    }

    // Check Invalid 4) Size
    if (IsStringValidLength(CommandToken[1], CommandToken[2]) == false) {
        return new InvalidCommand();
    }

    return new EraseRangeCommand(LimitToMinLBA(CommandToken[1]), LimitToMaxLBA(CommandToken[2]));
}

ShellCommand* ShellCommandFactory::MakeFlushCommand() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 1) {
        return new InvalidCommand();
    }

    return new FlushCommand();
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

ShellCommand* ShellCommandFactory::MakeCompareCommand() {
    // Check Invalid 1) Argument Length
    if (CommandToken.size() != 1) {
        return new InvalidCommand();
    }

    return new Compare();
}

bool ShellCommandFactory::IsStringDecimal(const std::string& str) {
    for (char ch = 0; ch < str.size(); ch++) {
        if ('0' > str[ch] || str[ch] > '9') {
            return false;
        }
    }
    return true;
}

bool ShellCommandFactory::IsStringHexadecimal(const std::string& str) {
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

bool ShellCommandFactory::IsStringValidLBA(const std::string& str) {
    int LBA = std::stoi(str);
    if ((ssddriver->GetMinLBA() <= LBA) && (LBA <= ssddriver->GetMaxLBA())) {
        return true;
    }
    return false;
}

bool ShellCommandFactory::IsStringValidLength(const std::string& str) {
    return (std::stoi(str) > 0);
}

bool ShellCommandFactory::IsStringValidLength(const std::string& strStartLBA, const std::string& strEndLBA)
{
    return ((std::stoi(strEndLBA) - std::stoi(strStartLBA)) > 0);
}


std::string ShellCommandFactory::LimitToMinLBA(const std::string& str) {
    int LBA = std::stoi(str);
    if (LBA < ssddriver->GetMinLBA()) {
        LBA = ssddriver->GetMinLBA();
    }

    return std::to_string(LBA);
}

std::string ShellCommandFactory::LimitToMaxLBA(const std::string& str) {
    int LBA = std::stoi(str);
    if (LBA > ssddriver->GetMaxLBA() + 1) {
        LBA = ssddriver->GetMaxLBA() + 1;
    }

    return std::to_string(LBA);
}
