// Copyright [2024] <CRA/BestReviewer>
#include <iostream>
#include <string>
#include "TestShell.h"
#include "RealSsdDriver.h"

int main() {
    TestShell TestShellApp;
    TestShellApp.SetSsdDriver(new RealSsdDriver());
    while (true) {
        std::string command;
        std::getline(std::cin, command);
        try {
            TestShellApp.Run(command);
        }
        catch (ExitTestShell) {
            break;
        }
    }
    return 0;
}
