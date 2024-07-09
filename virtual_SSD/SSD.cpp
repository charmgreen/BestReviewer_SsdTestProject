#include "SSD.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

std::string SSD::Read(int lba){
  if (isLbaOutOfRange(lba)) {
    throw invalid_argument("Invalid lab when reading the data.");
  }

  map<int, std::string> copyMemory = copyDataFromNandFile();

  if (copyMemory.find(lba) != copyMemory.end()) {
    string readData = copyMemory[lba];
    saveReadDataInResultFile(readData);
    return readData;
  }

  return "0x00000000";
}

void SSD::saveReadDataInResultFile(string readData) {
  ofstream resultFile("result.txt");
  resultFile << readData;
  resultFile.close();
}

map<int, string> SSD::copyDataFromNandFile() {
  ifstream nandFile("nand.txt");
  if (!nandFile.is_open()) {
    throw runtime_error("nand.txt does not exist.");
  }

  map<int, string> copyData;
  int lbaNum = 0;
  string readLine;

  while (!nandFile.eof()) {
    getline(nandFile, readLine);
    copyData.insert({lbaNum, readLine});
    lbaNum++;
  }
  nandFile.close();

  return copyData;
}


bool SSD::isLbaOutOfRange(int lba) { return lba < 0 || lba > 99; }

/// <summary>
/// class VirtualSSD : SSD {
//public:
// string Read(int lba) override {
//   if (isLbaOutOfRange(lba)) {
//     throw invalid_argument("Invalid lab when reading the data.");
//   }

//  copyMemory = copyDataFromNandFile();

//  if (isLbaDataExistInMemory(lba)) {
//    string readData = copyMemory[lba];
//    saveReadDataInResultFile(readData);
//    return readData;
//  }

//  return "0x00000000";
//}

//private:
//map<int, string> copyMemory;
//}
//;
/// </summary>

