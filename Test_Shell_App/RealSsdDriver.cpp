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
    std::string virtual_ssd_exe_path = "..\\x64\\Debug\\virtual_SSD.exe";
    virtual_ssd_exe_path += " ";
    virtual_ssd_exe_path += cmdLine;

    // b 프로젝트의 실행 파일을 시스템 호출로 실행
    int result = system(virtual_ssd_exe_path.c_str());

    if (result == 0) {
        // b.exe가 성공적으로 실행되었음
        std::cout << "b.exe executed successfully.\n";
    } else {
        // b.exe 실행에 실패했음
        std::cerr << "Failed to execute b.exe. Error code: " << result << '\n';
    }
}
