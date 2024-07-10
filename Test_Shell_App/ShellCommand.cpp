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
    throw ExitTestShell();
}

void HelpCommand::Run(SsdDriver* ssddriver) {
    std::string HelpMessage = "";

    HelpMessage += "[Help]\n";
    HelpMessage += "1. write {LBA} {Data}\n";
    HelpMessage += "2. read {LBA}\n";
    HelpMessage += "3. exit\n";
    HelpMessage += "4. help\n";
    HelpMessage += "5. fullwrite {Data}\n";
    HelpMessage += "6. fullread\n";
    HelpMessage += "7. testapp1\n";
    HelpMessage += "8. testapp2\n";
    HelpMessage += "{LBA} = {x is an integer | 0 <= x <= 99}\n";
    HelpMessage += "{Data} = {""0x[0-9A-F]""}\n";
    std::cout << HelpMessage;
}

FullWriteCommand::FullWriteCommand(std::string strData) {
    this->strData = strData;
}

void FullWriteCommand::Run(SsdDriver* ssddriver) {
    for (int LBA = MIN_LBA; LBA <= MAX_LBA; LBA++) {
        ssddriver->Write(LBA, this->strData);
    }
}

void FullReadCommand::Run(SsdDriver* ssddriver) {
    std::cout << "[FullRead]\n";
    for (int LBA = MIN_LBA; LBA <= MAX_LBA; LBA++) {
        std::string resultData = ssddriver->Read(LBA);
        std::cout << "[Read] LBA : " << std::to_string(LBA);
        std::cout << ", Data : " << resultData << std::endl;
    }
}

void TestApp1::Run(SsdDriver* ssddriver) {
    std::cout << "[TestApp1]\n";
    for (int LBA = MIN_LBA; LBA <= MAX_LBA; LBA++) {
        ssddriver->Write(LBA, "0x12341234");
    }
    for (int LBA = MIN_LBA; LBA <= MAX_LBA; LBA++) {
        std::string resultData = ssddriver->Read(LBA);
        std::cout << "[Read] LBA : " << std::to_string(LBA);
        std::cout << ", Data : " << resultData << std::endl;
    }
}

void TestApp2::Run(SsdDriver* ssddriver) {
    std::cout << "[TestApp2]\n";
    for (unsigned int LBA = 0; LBA <= 5; LBA++) {
        for (unsigned int cnt = 1; cnt <= 5; cnt++) {
            ssddriver->Write(LBA, "0xAAAABBBB");
        }
    }

    for (unsigned int LBA = 0; LBA <= 5; LBA++) {
        ssddriver->Write(LBA, "0x12345678");
    }

    for (int LBA = 0; LBA <= 5; LBA++) {
        std::string resultData = ssddriver->Read(LBA);
        std::cout << "[Read] LBA : " << std::to_string(LBA);
        std::cout << ", Data : " << resultData << std::endl;
    }
}
