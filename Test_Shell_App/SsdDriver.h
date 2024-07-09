// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <string>
class SsdDriver {
 public:
    virtual std::string Read(int LBA) = 0;
    virtual void Write(int LBA, std::string Data) = 0;
};
