// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include "SsdDriver.h"

class RealSsdDriver : public SsdDriver {
 public:
    unsigned int Read(int LBA) override;
    void Write(int LBA, unsigned int Data) override;
};
