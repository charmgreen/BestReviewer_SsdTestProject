// Copyright [2024] <CRA/BestReviewer>
#include <iostream>
#include <fstream>
#include <string>
#include "TestShell.h"
#include "RealSsdDriver.h"

using namespace std;

int main(int argc, char* argv[]) {
    TestShell TestShellApp;
    TestShellApp.SetSsdDriver(new RealSsdDriver());

    if (argc == 1)
    {
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
    else if (argc == 2)
    {
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

    return 0;
}
