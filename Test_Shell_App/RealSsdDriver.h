// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <iostream>
#include <string>
#include "SsdDriver.h"

using namespace std;

const int CONFIG_MAX_LBA = 100;

class CompareBufferMgr {
public:
    string GetCompareData(int LBA);
    void SetCompareData(int LBA, string Data);
private:
    string compareData[CONFIG_MAX_LBA];

    const string ERASE_DATA = "0x00000000";
};

class RealSsdDriver : public SsdDriver {
 public:
    RealSsdDriver();
    string Read(int LBA) override;
    void Write(int LBA, string Data) override;
    void Erase(int startLBA, int Size) override;
    void Flush() override;

    int GetMinLBA() override { return MIN_LBA; }
    int GetMaxLBA() override { return MAX_LBA; }
    string CmpBufRead(int LBA) override;
 private:
    void SystemCall(string cmdLine);

    CompareBufferMgr cmpBufMgr;

    const int MIN_LBA = 0;
    const int MAX_LBA = 99;
    const int ERASE_LBA_UNIT = 10;
};
