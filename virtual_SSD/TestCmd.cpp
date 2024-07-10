// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include "TestCmd.h"
#include "SSD.h"
#include "Parser.h"
#include "SSDInterface.h"

void TestCmd::Run(const std::string& strCommand) {
    parser = SetParser();
    ssd = SetSSD();
    cmd = parser->Parse(strCommand);

    if (cmd->Command == CmdType::Write) {
        _Write();
    }
    else if (cmd->Command == CmdType::Read) {
        _Read();
    }

    delete (parser);
    delete (ssd);
    delete (cmd);
}

Parser* TestCmd::SetParser() { 
    Parser* parser = new Parser();
    return parser;
}

SSDInterface* TestCmd::SetSSD() {
    return ssd;
}

void TestCmd::_Write() {
    ssd->Write(stoi(cmd->LBA), cmd->LBAData);
}

std::string TestCmd::_Read() {
    std::string res = ssd->Read(stoi(cmd->LBA));
    return res;
}
