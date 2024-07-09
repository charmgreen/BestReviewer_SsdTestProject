// Copyright [2024] <CRA/BestReviewer>
#include<stdexcept>
#include<fstream>
#include<iostream>
#include <sstream>
#include <string>
#include <map>
#include"SSD.h"

class LBARangeException : public std::exception {};

class DataRangeException : public std::exception {};

class NotExistNandFileException : public std::exception {};

void SSD::Write(const int& LBA, const std::string& data) {
    CheckWriteCondition(LBA, data);
    ReadMemory();
    UpdateMemory(LBA, data);
    StoreMemory();
}

void SSD::CheckWriteCondition(const int& LBA, const std::string& data) {
    CheckLBARange(LBA);
    CheckDataLength(data);
}

void SSD::StoreMemory() {
    std::ofstream writeFile(WriteFIleName);
    if (writeFile.is_open()) {
        for (int LBA = 0; LBA <= MAX_LBA; LBA++) {
            writeFile << LBA << " " << memory[LBA] << "\n";
        }
    }
}

void SSD::UpdateMemory(const int& LBA, const std::string& data) {
    memory[LBA] = data;
}

void SSD::ReadMemory() {
    std::string line;
    std::ifstream writeFIle(WriteFIleName);

    if (writeFIle.is_open()) {
        while (getline(writeFIle, line)) {
            int firstSpacePosition = line.find(' ');
            std::string LBA = line.substr(0, firstSpacePosition);
            int iLBA = stoi(LBA);
            std::string LBADATA = line.substr(firstSpacePosition + 1);
            UpdateMemory(iLBA, LBADATA);
        }
        writeFIle.close();
    } else {
        for (int i = 0; i <= MAX_LBA; i++) {
            memory[i] = InitialLBAData;
        }
    }
}

void SSD::CheckDataLength(const std::string& data) {
    if (data.length() != 10)
        throw DataRangeException();
}

void SSD::CheckLBARange(const int& LBA) {
    if (LBA < 0 || LBA > MAX_LBA)
        throw LBARangeException();
}

std::string SSD::Read(const int &LBA) {
  CheckLBARange(LBA);
  CheckExistNandFile();
  ReadMemory();
  return ReturnReadData(LBA);
}

void SSD::CheckExistNandFile() {
  std::ifstream nandFile(WriteFIleName);
  if (!nandFile.is_open()) {
    throw NotExistNandFileException();
  }
  nandFile.close();
}

const std::string &SSD::ReturnReadData(const int &LBA) {
  if (memory.find(LBA) != memory.end()) {
    WriteResultFile(LBA);
    return memory[LBA];
  }
  return InitialLBAData;
}

void SSD::WriteResultFile(const int &LBA) {
  std::ofstream resultFile(ReadFileName);
  resultFile << memory[LBA];
  resultFile.close();
}

