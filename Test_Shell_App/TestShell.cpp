// Copyright [2024] <CRA/BestReviewer>
#include "TestShell.h"
#include "SsdDriver.h"

void TestShell::Run(const std::string& strCommand) {
    CmdLine = factory.Parse(strCommand);
}

void TestShell::SetSsdDriver(SsdDriver* ssddriver) {
    this->ssddriver = ssddriver;
}
