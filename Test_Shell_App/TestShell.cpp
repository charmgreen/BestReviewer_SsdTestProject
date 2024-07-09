// Copyright [2024] <CRA/BestReviewer>
#include "TestShell.h"
#include "SsdDriver.h"
#include <string>

void TestShell::Run(const std::string& strCommand) {
    shellCommand = shellCommandFactory.Make(strCommand);

    shellCommand->Run(ssddriver);

    delete(shellCommand);
}

void TestShell::SetSsdDriver(SsdDriver* ssddriver) {
    this->ssddriver = ssddriver;
}

