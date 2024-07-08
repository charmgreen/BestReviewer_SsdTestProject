// Copyright [2024] <CRA/BestReviewer>
#include "TestShell.h"
#include "SsdDriver.h"

void TestShell::Run(const std::string& strCommand) {

    shellCommand = factory.Parse(strCommand);

    if (shellCommand->eCommand == ShellCmdType::Write) {
        _Write();
    }
    else if (shellCommand->eCommand == ShellCmdType::Read) {
        _Read();
    }
    else if (shellCommand->eCommand == ShellCmdType::Exit) {
        _Exit();
    }
    else if (shellCommand->eCommand == ShellCmdType::Help) {
        _Help();
    }
    else if (shellCommand->eCommand == ShellCmdType::FullWrite) {
        _FullWrite();
    }
    else if (shellCommand->eCommand == ShellCmdType::FullRead) {
        _FullRead();
    }
    else if (shellCommand->eCommand == ShellCmdType::TestApp1) {
        _TestApp1();
    }
    else if (shellCommand->eCommand == ShellCmdType::TestApp2) {
        _TestApp2();
    }
    delete(shellCommand);
}

void TestShell::SetSsdDriver(SsdDriver* ssddriver) {
    this->ssddriver = ssddriver;
}

void TestShell::_Write() {
    ssddriver->Write(shellCommand->LBA, shellCommand->Data);
}

void TestShell::_Read() {
    ssddriver->Read(shellCommand->LBA);
}

void TestShell::_Exit() {
}

void TestShell::_Help() {
}

void TestShell::_FullWrite() {
}

void TestShell::_FullRead() {
}

void TestShell::_TestApp1() {
}

void TestShell::_TestApp2() {
}
