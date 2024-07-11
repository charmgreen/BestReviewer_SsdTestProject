// Copyright [2024] <CRA/BestReviewer>
#include <iostream>
#include <fstream>
#include <cstdio> // std::remove
#include <string>
#include "TestShell.h"
#include "RealSsdDriver.h"

using namespace std;

const int COMMAND_MODE = 1;
const int SCRIPT_MODE = 2;

void CommandMode(void);
void ScriptMode(char* argv[]);
void RunScript(std::ifstream& runListFile);
void FormatSSD(void);

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

bool deleteFileIfExists(const string& file_path)
{
    ifstream file(file_path);

    // 파일이 존재하는지 확인
    if (file.good()) {
        file.close(); // 파일 스트림 닫기
        if (std::remove(file_path.c_str()) == 0) {
            return true; // 파일 삭제 성공
        }
        else {
            return false; // 파일 삭제 실패
        }
    }
    else {
        return false; // 파일이 존재하지 않음
    }
}

void FormatSSD(void)
{
    deleteFileIfExists("nand.txt");
    deleteFileIfExists("result.txt");
}

void CommandMode(void)
{
    TestShell TestShellApp;
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
    string scriptFileName;

    while (getline(runListFile, scriptFileName)) {
        string command;
        ifstream scriptFile(scriptFileName);

        TestShell TestShellApp;
        TestShellApp.SetSsdDriver(new RealSsdDriver());
        FormatSSD();

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

