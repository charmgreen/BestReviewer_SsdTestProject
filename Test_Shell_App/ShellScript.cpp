// Copyright [2024] <CRA/BestReviewer>
#include <iostream>
#include <string>
#include <fstream>
#include "ShellScript.h"

ShellScript::ShellScript(std::string filename) {
    this->filename = filename;
}

void ShellScript::Run(SsdDriver* ssddriver) {
    std::ifstream script(filename);
    if (!script.is_open()) {
        throw::std::runtime_error("Not found the script.");
    }
    try {
        while (!script.eof()) {
            std::string statement;
            std::getline(script, statement);

            ShellCommand* shellCommand;
            shellCommand = Make(statement);
            shellCommand->Run(ssddriver);
            delete(shellCommand);
        }
        script.close();
    }
    catch (...) {
        std::cout << "ERROR!" << std::endl;
    }
}
