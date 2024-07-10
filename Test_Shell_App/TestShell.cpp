// Copyright [2024] <CRA/BestReviewer>
#include "TestShell.h"
#include "SsdDriver.h"
#include <string>

void TestShell::Run(const std::string& strCommand) {
    shellCommand = shellCommandFactory.Make(strCommand);

    try {
        shellCommand->Run(ssddriver);
    }
    catch (ExitTestShell) {
        throw ExitTestShell();
    }

    delete(shellCommand);
}

void TestShell::SetSsdDriver(SsdDriver* ssddriver) {
    this->ssddriver = ssddriver;
    shellCommandFactory.SetSsdDriver(ssddriver);
}
