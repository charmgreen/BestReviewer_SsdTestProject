// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <iostream>
#include <string>
#include "SsdDriver.h"

class RealSsdDriver : public SsdDriver {
 public:
    std::string Read(int LBA) override;
    void Write(int LBA, std::string Data) override;
    void Erase(int startLBA, int Size) override;
    void Flush() override;

    int GetMinLBA() override { return MIN_LBA; }
    int GetMaxLBA() override { return MAX_LBA; }
 private:
    void SystemCall(std::string cmdLine);

    const int MIN_LBA = 0;
    const int MAX_LBA = 99;
    const int ERASE_LBA_UNIT = 10;
};
