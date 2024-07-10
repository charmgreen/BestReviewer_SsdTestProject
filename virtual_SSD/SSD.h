// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include<map>
#include<string>
#include"SSDInterface.h"

class SSD : public SSDInterface {
 public:
    virtual void Write(const int& LBA, const std::string& data) override;
    virtual void Read(const int &LBA) override;
    virtual void Erase(const int &LBA, const int &size) override;
    virtual void Flush() override;

 private:
    std::map<int, std::string> memory;
    std::string InitialLBAData{ "0x00000000" };
    std::string WriteFIleName{ "nand.txt" };
    std::string ReadFileName{ "result.txt" };
    std::string DataPreFix{ "0x" };
    const int MAX_LBA{ 99 };
    const int MIN_LBA{ 0 };
    const int InitialUpdateSize{1};

    void ProcessMemory(const int &LBA, const std::string data, const int &size);
    void ReadMemory();
    void UpdateMemory(const int &LBA, const std::string &data, const int &size);
    void StoreMemory();
    void CheckWriteCondition(const int& LBA, const std::string& data);
    void CheckEraseCondition(const int &LBA, const int &size);
    void CheckLBARange(const int& LBA);
    void CheckDataLength(const std::string& data);
    void CheckDataPreFix(const std::string& data);
    void CheckDataType(const std::string& data);
    void CheckEraseSizeRange(const int size);
    bool isHexData(const char& data);
    void WriteResultFile(const int& LBA);
};
