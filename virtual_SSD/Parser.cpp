// Copyright [2024] <CRA/BestReviewer>
#include<stdexcept>
#include"Parser.h"

class ArgsLengthNotMatchException : public std::exception {};

CmdStatus *Parser::Parse(const std::string &strCommand) {
    TokenArgument(strCommand);
    return UpdateCmdStatus();
}

void Parser::TokenArgument(const std::string& strCommand) {
    std::string token;
    size_t start = strCommand.find(' ', 0) + 1;
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
    else if (CommandToken[0] == ERASE_CMD) return UpdateEraseCmdStatus();
    return nullptr;
}

CmdStatus *Parser::UpdateWriteCmdStatus() {
    CheckWriteCommandToken();
    CmdStatus* result = new CmdStatus();
    result->Command = CmdType::Write;
    result->LBA = CommandToken[1];
    result->LBAData = CommandToken[2];
    return result;
}

CmdStatus* Parser::UpdateReadCmdStatus() {
    CheckReadCommandToken();
    CmdStatus* result = new CmdStatus();
    result->Command = CmdType::Read;
    result->LBA = CommandToken[1];
    return result;
}

CmdStatus *Parser::UpdateEraseCmdStatus() {
    CheckEraseCommandToken();
    CmdStatus *result = new CmdStatus();
    result->Command = CmdType::Erase;
    result->LBA = CommandToken[1];
    result->EraseSize = stoi(CommandToken[2]);
    return result;
}

void Parser::CheckWriteCommandToken() {
    if (CommandToken.size() != 3) {
        throw ArgsLengthNotMatchException();
    }
}

void Parser::CheckReadCommandToken() {
    if (CommandToken.size() != 2) {
        throw ArgsLengthNotMatchException();
    }
}

void Parser::CheckEraseCommandToken() {
    if (CommandToken.size() != 3) {
        throw ArgsLengthNotMatchException();
    }
}
