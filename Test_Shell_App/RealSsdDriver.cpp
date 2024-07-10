// Copyright 2024, Samsung
#include "RealSsdDriver.h"
#include <iostream>
#include <string>

std::string RealSsdDriver::Read(int LBA) {
    std::string cmdLine = "R " + std::to_string(LBA);
    SystemCall(cmdLine);
    return cmdLine;  // temp
}

void RealSsdDriver::Write(int LBA, std::string Data) {
    std::string cmdLine = "W " + std::to_string(LBA) + " " + Data;
    SystemCall(cmdLine);
}

void RealSsdDriver::SystemCall(std::string cmdLine) {
#ifdef _DEBUG
    std::string virtual_ssd_exe_path = "..\\x64\\Debug\\virtual_SSD.exe";
#else
    std::string virtual_ssd_exe_path = "virtual_SSD.exe";
#endif
    virtual_ssd_exe_path += " ";
    virtual_ssd_exe_path += cmdLine;

    int result = system(virtual_ssd_exe_path.c_str());

    if (result == 0) {
        std::cout << "virtual_SSD.exe executed successfully.\n";
    } else {
        std::cerr << "Failed to execute virtual_SSD.exe. Error code: " << result << '\n';
    }
}
