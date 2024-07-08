// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include "SsdDriver.h"

class RealSsdDriver : public SsdDriver {
 public:
    int Read(int LBA) override;
    void Write(int LBA, int Data) override;
};
