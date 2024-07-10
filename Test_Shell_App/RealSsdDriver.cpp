// Copyright 2024, Samsung
#include "RealSsdDriver.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string RealSsdDriver::Read(int LBA) {
    string cmdLine = "R " + to_string(LBA);
    SystemCall(cmdLine);
    string ReadFileName{ "result.txt" };
    ifstream resultFile(ReadFileName);
    string line;

    if (resultFile.is_open()) {
        while (getline(resultFile, line)) {
            cout << line << endl;
        }
        resultFile.close();
    }
    else {
        cerr << "Open Error " + ReadFileName << endl;
    }

    return cmdLine;
}

void RealSsdDriver::Write(int LBA, std::string Data) {
    string cmdLine = "W " + to_string(LBA) + " " + Data;
    SystemCall(cmdLine);
}

void RealSsdDriver::SystemCall(std::string cmdLine) {
#ifdef _DEBUG
    string virtual_ssd_exe_path = "..\\x64\\Debug\\virtual_SSD.exe";
#else
    string virtual_ssd_exe_path = "virtual_SSD.exe";
#endif
    virtual_ssd_exe_path += " ";
    virtual_ssd_exe_path += cmdLine;

    int result = system(virtual_ssd_exe_path.c_str());

    if (result)
    {
        cerr << "Failed to execute virtual_SSD.exe. Error code: " << result << '\n';
    }
}
