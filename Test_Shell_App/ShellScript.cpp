// Copyright [2024] <CRA/BestReviewer>
#include <iostream>
#include <string>
#include <fstream>
#include "ShellScript.h"

ShellScript::ShellScript(std::string filename, SsdDriver* ssddriver) {
    this->filename = filename;
    SetSsdDriver(ssddriver);
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

            ShellCommand* shellCommand = Make(statement);
            try {
                shellCommand->Run(ssddriver);

                delete(shellCommand);
            }
            catch (ExitCommand) {
                delete(shellCommand);
                throw;
            }
        }
        script.close();
    }
    catch (...) {
        std::cout << "ERROR!" << std::endl;
    }


}
