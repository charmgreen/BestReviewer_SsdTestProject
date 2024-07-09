// Copyright [2024] <CRA/BestReviewer>
#include <iostream>
#include <string>
#include "ShellCommand.h"
#include "SsdDriver.h"

void InvalidCommand::Run(SsdDriver* ssddriver) {
    std::cout << "INVALID COMMAND" << std::endl;
}

WriteCommand::WriteCommand(std::string strLBA, std::string strData) {
    this->strLBA = strLBA;
    this->nLBA = std::stoi(strLBA);
    this->strData = strData;
}

void WriteCommand::Run(SsdDriver* ssddriver) {
    ssddriver->Write(this->nLBA, this->strData);
    std::cout << "[Write] LBA : " << this->strLBA;
    std::cout << ", Data : " << this->strData << std::endl;
}

ReadCommand::ReadCommand(std::string strLBA) {
    this->strLBA = strLBA;
    this->nLBA = std::stoi(strLBA);
}

void ReadCommand::Run(SsdDriver* ssddriver) {
    std::string resultData = ssddriver->Read(this->nLBA);
    std::cout << "[Read] LBA : " << this->strLBA;
    std::cout << ", Data : " << resultData << std::endl;
}

void ExitCommand::Run(SsdDriver* ssddriver) {
    std::cout << "[Exit] Quit Shell" << std::endl;
}

void HelpCommand::Run(SsdDriver* ssddriver) {
    std::cout << "[Help]" << std::endl;
}

FullWriteCommand::FullWriteCommand(std::string strData) {
    this->strData = strData;
}

void FullWriteCommand::Run(SsdDriver* ssddriver) {
    std::cout << "[FullWrite]\n";
    for (int LBA = 0; LBA <= 99; LBA++) {
        ssddriver->Write(LBA, this->strData);
    }
}

void FullReadCommand::Run(SsdDriver* ssddriver) {
    std::cout << "[FullRead]\n";
    for (int LBA = 0; LBA <= 99; LBA++) {
        std::string resultData = ssddriver->Read(LBA);
        std::cout << "[Read] LBA : " << std::to_string(LBA);
        std::cout << ", Data : " << resultData << std::endl;
    }
}
