// Copyright [2024] <CRA/BestReviewer>
#include <iostream>
#include <string>
#include "ShellCommand.h"
#include "SsdDriver.h"
#include "../Logger/logger.cpp"

void InvalidCommand::Run(SsdDriver* ssddriver) {
    Logger::getInstance().print(__FUNCTION__, "");
    std::cout << "INVALID COMMAND" << std::endl;
}

WriteCommand::WriteCommand(std::string strLBA, std::string strData) {
    this->strLBA = strLBA;
    this->nLBA = std::stoi(strLBA);
    this->strData = strData;
}

void WriteCommand::Run(SsdDriver* ssddriver) {
    Logger::getInstance().print(__FUNCTION__, "execute write command");
    ssddriver->Write(this->nLBA, this->strData);
}

ReadCommand::ReadCommand(std::string strLBA) {
    this->strLBA = strLBA;
    this->nLBA = std::stoi(strLBA);
}

void ReadCommand::Run(SsdDriver* ssddriver) {
    Logger::getInstance().print(__FUNCTION__, "execute read command");
    std::string resultData = ssddriver->Read(this->nLBA);
    std::cout << "[Read] LBA : " << this->strLBA;
    std::cout << ", Data : " << resultData << std::endl;
}

EraseCommand::EraseCommand(std::string strStartLBA, std::string strSize) {
    this->strStartLBA = strStartLBA;
    this->strSize = strSize;
    this->nStartLBA = std::stoi(strStartLBA);
    this->nSize = std::stoi(strSize);
}

void EraseCommand::Run(SsdDriver* ssddriver) {
    Logger::getInstance().print(__FUNCTION__, "execute erase command");
    ssddriver->Erase(this->nStartLBA, this->nSize);
}

EraseRangeCommand::EraseRangeCommand(std::string strStartLBA, std::string strEndLBA) {
    this->strStartLBA = strStartLBA;
    this->strEndLBA = strEndLBA;
    this->nStartLBA = std::stoi(strStartLBA);
    this->nEndLBA = std::stoi(strEndLBA);
}

void EraseRangeCommand::Run(SsdDriver* ssddriver) {
    Logger::getInstance().print(__FUNCTION__, "execute erase range command");
    ssddriver->Erase(this->nStartLBA, this->nEndLBA - this->nStartLBA);
}

void FlushCommand::Run(SsdDriver* ssddriver) {
    Logger::getInstance().print(__FUNCTION__, "execute flush command");
    ssddriver->Flush();
}

void ExitCommand::Run(SsdDriver* ssddriver) {
    Logger::getInstance().print(__FUNCTION__, "");
    std::cout << "[Exit] Quit Shell" << std::endl;
    throw ExitTestShell();
}

void HelpCommand::Run(SsdDriver* ssddriver) {
    Logger::getInstance().print(__FUNCTION__, "print supported commands");
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
    HelpMessage += "9. erase {LBA} {Size}\n";
    HelpMessage += "10. erase_range {LBA} {LBA}\n";
    HelpMessage += "11. flush\n";
    HelpMessage += "{LBA} = {x is an integer | 0 <= x <= 99}\n";
    HelpMessage += "{Data} = {""0x[0-9A-F]""}\n";
    std::cout << HelpMessage;
}

FullWriteCommand::FullWriteCommand(std::string strData) {
    this->strData = strData;
}

void FullWriteCommand::Run(SsdDriver* ssddriver) {
    Logger::getInstance().print(__FUNCTION__, "execute full write command");
    for (int LBA = ssddriver->GetMinLBA(); LBA <= ssddriver->GetMaxLBA(); LBA++) {
        ssddriver->Write(LBA, this->strData);
    }
}

void FullReadCommand::Run(SsdDriver* ssddriver) {
    Logger::getInstance().print(__FUNCTION__, "execute full read command");
    std::cout << "[FullRead]\n";
    for (int LBA = ssddriver->GetMinLBA(); LBA <= ssddriver->GetMaxLBA(); LBA++) {
        std::string resultData = ssddriver->Read(LBA);
        std::cout << "[Read] LBA : " << std::to_string(LBA);
        std::cout << ", Data : " << resultData << std::endl;
    }
}

void TestApp1::Run(SsdDriver* ssddriver) {
    Logger::getInstance().print(__FUNCTION__, "execute testapp1 scenario");
    std::cout << "[TestApp1]\n";
    for (int LBA = ssddriver->GetMinLBA(); LBA <= ssddriver->GetMaxLBA(); LBA++) {
        ssddriver->Write(LBA, "0x12341234");
    }
    for (int LBA = ssddriver->GetMinLBA(); LBA <= ssddriver->GetMaxLBA(); LBA++) {
        std::string resultData = ssddriver->Read(LBA);
        std::cout << "[Read] LBA : " << std::to_string(LBA);
        std::cout << ", Data : " << resultData << std::endl;
    }
}

void TestApp2::Run(SsdDriver* ssddriver) {
    Logger::getInstance().print(__FUNCTION__, "execute testapp2 scenario");
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

void Compare::Run(SsdDriver* ssddriver) {
    std::cout << "[Compare]\n";
    for (int LBA = MIN_LBA; LBA <= MAX_LBA; LBA++) {
        std::string resultData = ssddriver->Read(LBA);
        std::string compareData = ssddriver->CmpBufRead(LBA);
        if (resultData != compareData)
        {
            std::cout << "Fail\n";
            throw ExceptionCompareFail();
        }
    }

    std::cout << "Pass\n";
    return;
}

