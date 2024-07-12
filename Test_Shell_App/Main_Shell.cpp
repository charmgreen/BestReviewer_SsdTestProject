// Copyright [2024] <CRA/BestReviewer>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Shell.h"
#include "RealSsdDriver.h"
#include "../Logger/logger.cpp"
#include <chrono>

using namespace std;

const int COMMAND_MODE = 1;
const int SCRIPT_MODE = 2;

void CommandMode(void);
void ScriptMode(char* argv[]);
void RunScript(ifstream& runListFile);
void FormatSSD(void);

int main(int argc, char* argv[]) {
    if (argc == COMMAND_MODE) {
        CommandMode();
    }
    else if (argc == SCRIPT_MODE) {
        ScriptMode(argv);
    }

    return 0;
}

bool deleteFileIfExists(const string& file_path) {
    ifstream file(file_path);

    if (file.good()) {
        file.close();
        if (remove(file_path.c_str()) == 0) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

void FormatSSD(void) {
    deleteFileIfExists("nand.txt");
    deleteFileIfExists("result.txt");
    deleteFileIfExists("buffer.txt");
}

void CommandMode(void) {
    LOG_PRINT("Execute the input command being supported");
    Shell TestShellApp;
    TestShellApp.SetSsdDriver(new RealSsdDriver());

    FormatSSD();
    string command;
    while (true) {
        cout << "> ";
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

void ScriptMode(char* argv[]) {
    LOG_PRINT("Verify the script exists");
    string inputArg = argv[1];
    string strRunListFile{ inputArg };
    ifstream runListFile(strRunListFile);

    if (runListFile.is_open()) {
        RunScript(runListFile);
        runListFile.close();
    }
    else {
        cerr << "script file open error " << strRunListFile << endl;
    }
}

void RunScript(ifstream& runListFile)
{
    LOG_PRINT("Read the outer script");
    string strScriptFile;

    while (getline(runListFile, strScriptFile)) {
        ifstream scriptFile(strScriptFile);
        Shell TestShellApp;
        TestShellApp.SetSsdDriver(new RealSsdDriver());
        FormatSSD();

        if (scriptFile.is_open()) {
            stringstream actualOutput;
            streambuf* backup_cout;
            string command;
            bool bIsPass = true;

            cout << strScriptFile << " --- Run ... ";
            backup_cout = cout.rdbuf(actualOutput.rdbuf());

            while (getline(scriptFile, command)) {
                try {
                    TestShellApp.Run(command);
                }
                catch (ExitTestShell) {
                    break;
                }
                catch (ExceptionCompareFail) {
                    bool bIsPass = false;
                    break;
                }
            }

            scriptFile.close();
            cout.rdbuf(backup_cout);

            if (bIsPass) {
                cout << "Pass" << endl;
            }
            else {
                cout << "FAIL!" << endl;
            }
        }
        else {
            cerr << "script file open error " << strScriptFile << endl;
        }
    }
}
