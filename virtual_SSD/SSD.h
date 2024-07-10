// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include<map>
#include<string>

class SSD {
 public:
  virtual void Write(const int& LBA, const std::string& data);
  virtual std::string Read(const int& LBA);

 private:
  std::map<int, std::string> memory;
  std::string InitialLBAData{ "0x00000000" };
  std::string WriteFIleName{ "nand.txt" };
  std::string ReadFileName{ "result.txt" };
  std::string DataPreFix{ "0x" };
  const int MAX_LBA{ 99 };
  const int MIN_LBA{ 0 };

  void ReadMemory();
  void UpdateMemory(const int& LBA, const std::string& data);
  void StoreMemory();
  void CheckWriteCondition(const int& LBA, const std::string& data);
  void CheckLBARange(const int& LBA);
  void CheckDataLength(const std::string& data);
  void CheckDataPreFix(const std::string& data);
  void CheckDataType(const std::string& data);
  bool isHexData(const char& data);
  const std::string& ReturnReadData(const int& LBA);
  void WriteResultFile(const int& LBA);
};
