// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include "TestCmd.h"
#include "SSD.h"
#include "Parser.h"
#include "SSDInterface.h"

void TestCmd::Run(const std::string& strCommand) {
    cmd = parser->Parse(strCommand);

    if (cmd->Command == CmdType::Write) {
        _Write();
    }
    else if (cmd->Command == CmdType::Read) {
        _Read();
    }
}

void TestCmd::_Write() {
    ssd->Write(stoi(cmd->LBA), cmd->LBAData);
}

std::string TestCmd::_Read() {
    std::string res = ssd->Read(stoi(cmd->LBA));
    return res;
}
