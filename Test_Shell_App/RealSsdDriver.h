// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include "SsdDriver.h"
#include <iostream>

class RealSsdDriver : public SsdDriver {
 public:
    void SystemCall(std::string cmdLine);
    std::string Read(int LBA) override;
    void Write(int LBA, std::string Data) override;
};
