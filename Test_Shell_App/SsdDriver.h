// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <string>
class SsdDriver {
 public:
    virtual std::string Read(int LBA) = 0;
    virtual void Write(int LBA, std::string Data) = 0;
    virtual void Erase(int startLBA, int Size) = 0;
    virtual void Flush() = 0;
    virtual std::string CmpBufRead(int LBA) = 0;

    virtual int GetMinLBA() = 0;
    virtual int GetMaxLBA() = 0;
};
