// Copyright [2024] <CRA/BestReviewer>
#include <iostream>
#include <fstream>
#include <string>
#include "TestShell.h"
#include "RealSsdDriver.h"

using namespace std;

const int COMMAND_MODE = 1;
const int SCRIPT_MODE = 2;

void CommandMode();
void ScriptMode(char* argv[]);

int main(int argc, char* argv[]) {
    if (argc == COMMAND_MODE)
    {
        CommandMode();
    }
    else if (argc == SCRIPT_MODE)
    {
        ScriptMode(argv);
    }

    return 0;
}

void CommandMode()
{
    TestShell TestShellApp;
    TestShellApp.SetSsdDriver(new RealSsdDriver());

    while (true) {
        string command;
        getline(cin, command);
        try {
            TestShellApp.Run(command);
        }
        catch (ExitTestShell) {
            break;
        }
    }
}

void ScriptMode(char* argv[])
{
    TestShell TestShellApp;
    TestShellApp.SetSsdDriver(new RealSsdDriver());
    string inputArg = argv[1];
    string ReadFileName{ inputArg };
    ifstream resultFile(ReadFileName);
    string command;

    if (resultFile.is_open()) {
        while (getline(resultFile, command)) {
            cout << command << endl;
            try {
                TestShellApp.Run(command);
            }
            catch (ExitTestShell) {
                break;
            }
        }
        resultFile.close();
    }
    else {
        cerr << "Open Error " + ReadFileName << endl;
    }
}

