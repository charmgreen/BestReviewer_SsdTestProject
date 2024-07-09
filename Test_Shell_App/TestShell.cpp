// Copyright [2024] <CRA/BestReviewer>
#include "TestShell.h"
#include "SsdDriver.h"
#include <string>
#include <fstream>

void TestShell::Run(const std::string& strCommand) {
    shellCommand = factory.Parse(strCommand);
    if (shellCommand->IsInvalid == true) {
        _InvalidCommand();
    } else if (shellCommand->eCommand == ShellCmdType::Write) {
        _Write();
    } else if (shellCommand->eCommand == ShellCmdType::Read) {
        _Read();
    } else if (shellCommand->eCommand == ShellCmdType::Exit) {
        _Exit();
    } else if (shellCommand->eCommand == ShellCmdType::Help) {
        _Help();
    } else if (shellCommand->eCommand == ShellCmdType::FullWrite) {
        _FullWrite();
    } else if (shellCommand->eCommand == ShellCmdType::FullRead) {
        _FullRead();
    } else if (shellCommand->eCommand == ShellCmdType::TestApp1) {
        _TestApp1();
    } else if (shellCommand->eCommand == ShellCmdType::TestApp2) {
        _TestApp2();
    }

    delete(shellCommand);
}

void TestShell::SetSsdDriver(SsdDriver* ssddriver) {
    this->ssddriver = ssddriver;
}

void TestShell::_InvalidCommand() {
    std::cout << "INVALID COMMAND" << std::endl;
}

void TestShell::_Write() {
    ssddriver->Write(shellCommand->LBA, shellCommand->strData);
    std::cout << "[Write] LBA : " << shellCommand->strLBA;
    std::cout << ", Data : " << shellCommand->strData << std::endl;
}

void TestShell::_Read() {
    std::string resultData = ssddriver->Read(shellCommand->LBA);
    std::cout << "[Read] LBA : " << shellCommand->strLBA;
    std::cout << ", Data : " << resultData << std::endl;
}

void TestShell::_Exit() {
    std::cout << "[Exit] Quit Shell" << std::endl;
}

void TestShell::_Help() {
    std::cout << "[Help]" << std::endl;
}

void TestShell::_FullWrite() {
    std::cout << "[FullWrite]\n";
    for (int LBA = 0; LBA <= 99; LBA++) {
        ssddriver->Write(LBA, shellCommand->strData);
    }
}

void TestShell::_FullRead() {
    std::cout << "[FullRead]\n";
    for (int LBA = 0; LBA <= 99; LBA++) {
        std::string resultData = ssddriver->Read(LBA);
        std::cout << "[Read] LBA : " << std::to_string(LBA);
        std::cout << ", Data : " << resultData << std::endl;
    }
}

void TestShell::_TestApp1() {
    std::string filename("testapp1.txt");
    // std::cout << "Enter the script to execute>";
    // std::getline(std::cin, filename);
    _ExecuteScript(filename);
}

void TestShell::_TestApp2() {
    std::string filename("testapp2.txt");
    // std::cout << "Enter the script to execute>";
    // std::getline(std::cin, filename);
    _ExecuteScript(filename);
}

void TestShell::_ExecuteScript(const std::string& filename) {
    std::ifstream script(filename);
    if (!script.is_open()) {
        throw::std::runtime_error("Not found the script.");
    }
    try {
        while (!script.eof()) {
            std::string statement;
            std::getline(script, statement);
            TestShell::Run(statement);
        }
        script.close();
    } catch (...) {
        std::cout << "ERROR!" << std::endl;
    }
}
