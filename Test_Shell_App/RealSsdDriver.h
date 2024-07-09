// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <iostream>
#include <string>
#include "SsdDriver.h"

class RealSsdDriver : public SsdDriver {
 public:
    void SystemCall(std::string cmdLine);
    std::string Read(int LBA) override;
    void Write(int LBA, std::string Data) override;
};
