// Copyright [2024] <CRA/BestReviewer>
#include<stdexcept>
#include<fstream>
#include<iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include"SSD.h"

class LBARangeException : public std::exception {};
class DataRangeException : public std::exception {};
class DataPreFIxException : public std::exception {};
class DataTypeException : public std::exception {};
class EraseSizeException : public std::exception {};

void SSD::Write(const int& LBA, const std::string& data) {
    CheckWriteCondition(LBA, data);
    StoreCommand(LBA, data, InitialUpdateSize);
}

void SSD::Read(const int& LBA) {
    CheckLBARange(LBA);
    ReadMemory();
    WriteResultFile(LBA);
}

void SSD::Erase(const int &LBA, const int &size) {
    CheckEraseCondition(LBA, size);
    StoreCommand(LBA, InitialLBAData, InitialUpdateSize);
}

void SSD::Flush() {
}

void SSD::StoreCommand(const int& LBA, const std::string data, const int& size) {
    std::vector<std::string> lines = ReadFile(CommandBufferFileName);
    lines.push_back(std::to_string(LBA) + " " + data + " " + std::to_string(size));
    WriteFile(CommandBufferFileName, lines);
    CheckFlush(lines.size());
}

void SSD::CheckFlush(const int& bufferSize) {
    if (bufferSize == 10) {
        Flush();
    }
}

void SSD::WriteFile(std::string FileName, std::vector<std::string>& lines) {
    std::ofstream commandBufferWriteFile(FileName);
    if (commandBufferWriteFile.is_open()) {
        for (const auto& line : lines) {
            commandBufferWriteFile << line << "\n";
        }
        commandBufferWriteFile.close();
    }
}

std::vector<std::string> SSD::ReadFile(std::string FileName) {
    std::vector<std::string> lines;
    std::ifstream commandBufferReadFile(FileName);
    std::string line;

    if (commandBufferReadFile.is_open()) {
        while (getline(commandBufferReadFile, line)) {
            lines.push_back(line);
        }
        commandBufferReadFile.close();
    }

    return lines;
}

void SSD::ProcessMemory(const int &LBA, const std::string data, const int &size) {
    ReadMemory();
    UpdateMemory(LBA, data, size);
    StoreMemory();
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
            UpdateMemory(iLBA, LBADATA, InitialUpdateSize);
        }
        writeFIle.close();
    } else {
        for (int i = 0; i <= MAX_LBA; i++) {
            memory[i] = InitialLBAData;
        }
    }
}

void SSD::UpdateMemory(const int &LBA, const std::string &data, const int &size) {
    int endLBA = LBA + size;
    endLBA = endLBA > MAX_LBA ? MAX_LBA + 1 : endLBA;
    for (int iLBA = LBA; iLBA < endLBA; iLBA++) {
        memory[iLBA] = data;
    }
}

void SSD::StoreMemory() {
    std::ofstream writeFile(WriteFIleName);
    if (writeFile.is_open()) {
        for (int LBA = 0; LBA <= MAX_LBA; LBA++) {
            writeFile << LBA << " " << memory[LBA] << "\n";
        }
        writeFile.close();
    }
}

void SSD::CheckWriteCondition(const int& LBA, const std::string& data) {
    CheckLBARange(LBA);
    CheckDataLength(data);
    CheckDataPreFix(data);
    CheckDataType(data);
}

void SSD::CheckEraseCondition(const int &LBA, const int &size) {
    CheckLBARange(LBA);
    CheckEraseSizeRange(size);
}

void SSD::CheckLBARange(const int& LBA) {
    if (LBA < 0 || LBA > MAX_LBA)
        throw LBARangeException();
}

void SSD::CheckDataLength(const std::string& data) {
    if (data.length() != 10)
        throw DataRangeException();
}

void SSD::CheckDataPreFix(const std::string& data) {
    if (data.substr(0, 2) != DataPreFix)
        throw DataPreFIxException();
}

void SSD::CheckDataType(const std::string& data) {
    for (int i = 2; i < data.length(); i++) {
        if (isHexData(data[i]))continue;
        throw DataTypeException();
    }
}

void SSD::CheckEraseSizeRange(const int size) {
    if (size > 10 || size < 0)
        throw EraseSizeException();
}

bool SSD::isHexData(const char& data) {
    return (0 <= data - '0' && data - '0' < 10)
        || (0 <= data - 'A' && data - 'A' < 6);
}

void SSD::WriteResultFile(const int &LBA) {
    if (memory.find(LBA) != memory.end()) {
        std::ofstream resultFile(ReadFileName);
        resultFile << memory[LBA];
        resultFile.close();
    }
}
