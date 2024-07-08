// "Copyright [2024] <doyun kim>"

#pragma once
#include<map>
#include<string>

class SSD {
 public:
  virtual void Read();
  virtual void Write(const int& LBA, const std::string& data);
  void CheckWriteCondition(const int& LBA, const std::string& data);
  void StoreMemory();
  void UpdateMemory(const int& LBA, const std::string& data);
  void ReadMemory();
  void CheckDataLength(const std::string& data);
  void CheckLBARange(const int& LBA);

 private:
  std::map<int, std::string> memory;
  std::string InitialLBAData{ "0x00000000" };
  std::string WriteFIleName{ "nand.txt" };
  const int MAX_LBA{ 99 };
  const int MIN_LBA{ 0 };
};