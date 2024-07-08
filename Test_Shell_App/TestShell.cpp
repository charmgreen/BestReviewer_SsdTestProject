#include "TestShell.h"
#include "SsdDriver.h"

bool TestShell::GetUserInput(char str[]) {
    char* nextcmdLine = nullptr;
    char* cmdLine = strtok_s(str, "\n", &nextcmdLine);

    while (cmdLine != NULL) {
        RawInput.push_back(std::string(cmdLine));
        cmdLine = strtok_s(NULL, "\n", &nextcmdLine);
    }

    for (const auto& t : RawInput) {
        std::cout << t << std::endl;
    }
    return false;
}

void TestShell::Run() {
    // do nothing
}

void TestShell::SetSsdDriver(SsdDriver* ssddriver) {
    this->ssddriver = ssddriver;
}
