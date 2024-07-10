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

void RunScript(std::ifstream& runListFile);

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
    string command;
    while (true) {
        command.clear();
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
    string inputArg = argv[1];
    string ReadFileName{ inputArg };
    ifstream runListFile(ReadFileName);

    if (runListFile.is_open()) {

        RunScript(runListFile);

        runListFile.close();
    }
    else {
        cerr << "read Open Error " + ReadFileName << endl;
    }
}

void RunScript(ifstream& runListFile)
{
    TestShell TestShellApp;
    TestShellApp.SetSsdDriver(new RealSsdDriver());
    string scriptFileName;

    while (getline(runListFile, scriptFileName)) {
        string command;
        ifstream scriptFile(scriptFileName);

        if (scriptFile.is_open()) {
            while (getline(scriptFile, command)) {
                cout << command << endl;
                try {
                    TestShellApp.Run(command);
                }
                catch (ExitTestShell) {
                    break;
                }
            }
            scriptFile.close();
        }

        cout << scriptFileName << endl;
    }
}

