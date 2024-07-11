// Copyright 2024, Samsung
#include "RealSsdDriver.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../Logger/logger.cpp"

using namespace std;

void CompareBufferMgr::SetCompareData(int LBA, string Data) {
    if (LBA >= 0 && LBA < CONFIG_MAX_LBA) {
        compareData[LBA] = Data;
    }
}

string CompareBufferMgr::GetCompareData(int LBA) {
    if (LBA >= 0 && LBA < CONFIG_MAX_LBA) {
        return compareData[LBA];
    }
    else {
        return ERASE_DATA;
    }
}

RealSsdDriver::RealSsdDriver() {
    for (int i = 0; i < CONFIG_MAX_LBA; i++) {
        cmpBufMgr.SetCompareData(i, "0x00000000");
    }
}

string RealSsdDriver::Read(int LBA) {
    LOG_PRINT("Read from LBA");
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

void RealSsdDriver::Write(int LBA, string Data) {
    LOG_PRINT("Write a data to LBA");
    string cmdLine = "W " + to_string(LBA) + " " + Data;
    cmpBufMgr.SetCompareData(LBA, Data);
    SystemCall(cmdLine);
}

void RealSsdDriver::Erase(int startLBA, int Size) {
    LOG_PRINT("Erase data in specific area");
    int LBA = startLBA;
    while (Size > 0) {
        int EraseUnitSize = ((ERASE_LBA_UNIT < Size) ? (ERASE_LBA_UNIT) : (Size));
        string cmdLine = "E " + to_string(LBA);
        cmdLine += " " + to_string(EraseUnitSize);
        for (int i = LBA; i < LBA + EraseUnitSize; i++)
        {
            cmpBufMgr.SetCompareData(i, "0x00000000");
        }
        SystemCall(cmdLine);
        Size -= EraseUnitSize;
        LBA += EraseUnitSize;
    }
}

void RealSsdDriver::Flush() {
    LOG_PRINT("Execute commands in 'Command Buffer'");
    SystemCall("F");
}

string RealSsdDriver::CmpBufRead(int LBA) {
    return cmpBufMgr.GetCompareData(LBA);
}

void RealSsdDriver::SystemCall(string cmdLine) {
    LOG_PRINT("Execute SSD.exe with a command");
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
