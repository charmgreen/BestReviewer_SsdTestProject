// Copyright [2024] <CRA/BestReviewer>
#include "TestCmd.h"
#include "SSD.h"
#include "Parser.h"

void TestCmd::Run(const std::string& strCommand) {
    parser = SetParser();
    cmd = parser->Parse(strCommand);

    if (cmd->Command == CmdType::Write) {
        _Write();
    }
    else if (cmd->Command == CmdType::Read) {
        _Read();
    }
    
    delete(cmd);
}

void TestCmd::_Write() {
    ssd->Write(stoi(cmd->LBA), cmd->LBAData);
}

std::string TestCmd::_Read() {
    ssd->Read(stoi(cmd->LBA));
}
