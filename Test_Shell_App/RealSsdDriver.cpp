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
        getline(resultFile, line);
        resultFile.close();
    }
    else {
        cerr << "Open Error " + ReadFileName << endl;
    }

    return line;
}

void RealSsdDriver::Write(int LBA, std::string Data) {
    string cmdLine = "W " + to_string(LBA) + " " + Data;
    SystemCall(cmdLine);
}

void RealSsdDriver::Erase(int startLBA, int Size) {
    int LBA = startLBA;
    while (Size > 0) {
        int EraseUnitSize = ((ERASE_LBA_UNIT < Size) ? (ERASE_LBA_UNIT) : (Size));
        std::string cmdLine = "E " + std::to_string(LBA) + " " + std::to_string(EraseUnitSize);
        SystemCall(cmdLine);
        Size -= EraseUnitSize;
        LBA += EraseUnitSize;
    }
}

void RealSsdDriver::Flush() {
    SystemCall("F ");
}

void RealSsdDriver::SystemCall(std::string cmdLine) {
#ifdef _DEBUG
    string ssd_exe_path = "..\\x64\\Debug\\SSD.exe";
#else
    string ssd_exe_path = "SSD.exe";
#endif
    ssd_exe_path += " ";
    ssd_exe_path += cmdLine;

    int result = system(ssd_exe_path.c_str());

    if (result)
    {
        cerr << "Failed to execute SSD.exe. Error code: " << result << '\n';
    }
}
