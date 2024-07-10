// Copyright [2024] <CRA/BestReviewer>
#include"Parser.h"

void Parser::TokenArgument(const std::string& strCommand) {
    std::string token;
    size_t start = 0;
    size_t end = start;

    while ((end = strCommand.find(' ', start)) != std::string::npos) {
        token = strCommand.substr(start, end - start);
        CommandToken.push_back(token);
        start = end + 1;
    }

    token = strCommand.substr(start);
    CommandToken.push_back(token);
}

CmdStatus* Parser::UpdateCmdStatus() {
    if (CommandToken[0] == WRITE_CMD) return UpdateWriteCmdStatus();
    else if (CommandToken[0] == READ_CMD) return UpdateReadCmdStatus();
    return nullptr;
}

CmdStatus* Parser::UpdateWriteCmdStatus() {
    CmdStatus* result = new CmdStatus();
    result->Command = CmdType::Write;
    result->LBA = CommandToken[1];
    result->LBAData = CommandToken[2];
    return result;
}

CmdStatus* Parser::UpdateReadCmdStatus() {
    CmdStatus* result = new CmdStatus();
    result->Command = CmdType::Read;
    result->LBA = CommandToken[1];
    return result;
}

CmdStatus* Parser::Parse(const std::string& strCommand) {
    TokenArgument(strCommand);
    return UpdateCmdStatus();
}
