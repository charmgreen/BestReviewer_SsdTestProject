// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include "SSDCommand.h"
#include "SSD.h"
#include "Parser.h"
#include "SSDInterface.h"

void SSDCommand::Run(const std::string& strCommand) {
    cmd = parser->Parse(strCommand);

    if (cmd->Command == CmdType::Write) {
        _Write();
    } else if (cmd->Command == CmdType::Read) {
        _Read();
    } else if (cmd->Command == CmdType::Erase) {
        _Erase();
    }
}

void SSDCommand::_Write() {
    ssd->Write(stoi(cmd->LBA), cmd->LBAData);
}

std::string SSDCommand::_Read() {
    std::string res = ssd->Read(stoi(cmd->LBA));
    return res;
}

void SSDCommand::_Erase() { 
    //ssd->Erase(stoi(cmd->LBA), cmd->SizeData); 
}
