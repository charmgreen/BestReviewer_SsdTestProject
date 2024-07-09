// Copyright [2024] <CRA/BestReviewer>
#include "TestShell.h"
#include "SsdDriver.h"

void TestShell::Run(const std::string& strCommand) {

    shellCommand = factory.Parse(strCommand);
    if (shellCommand->IsInvalid == true) {
        std::cout << "INVALID COMMAND" << std::endl;
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

void TestShell::_Write() {
    ssddriver->Write(shellCommand->LBA, shellCommand->strData);
    // std::cout << "[Write] LBA : " << shellCommand->strLBA << " Data : " << shellCommand->strData << std::endl;
}

void TestShell::_Read() {
    ssddriver->Read(shellCommand->LBA);
}

void TestShell::_Exit() {
}

void TestShell::_Help() {
}

void TestShell::_FullWrite() {
    for (int LBA = 0; LBA <= 99; LBA++) {
        ssddriver->Write(LBA, shellCommand->strData);
    }
}

void TestShell::_FullRead() {
    for (int LBA = 0; LBA <= 99; LBA++) {
        ssddriver->Read(LBA);
    }
}

void TestShell::_TestApp1() {
}

void TestShell::_TestApp2() {
}
