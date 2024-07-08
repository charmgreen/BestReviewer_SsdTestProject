#include "SSD.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

class VirtualSSD : SSD {
public:
  void saveReadDataInResultFile(string readData) {
    ofstream resultFile("result.txt");
    resultFile << readData;
    resultFile.close();
  }

  map<int, string> copyDataFromNandFile() {
    ifstream nandFile("nand.txt");
    if (!nandFile.is_open()) {
        throw runtime_error("nand.txt does not exist.");
    }

    map<int, string> copyData;
    int labNum = 0;
    string readLine;

    while (!nandFile.eof()) {
      getline(nandFile, readLine);
      copyData.insert({labNum, readLine});
      labNum++;
    }
    nandFile.close();

    return copyData;
  }

  bool isLabDataExistInMemory(int lab) {
    return copyMemory.find(lab) != copyMemory.end();
  }

  bool isLabOutOfRange(int lab) { 
    return lab < 0 || lab > 99; 
  }

  string SSD::Read(int lab) override {
    if (isLabOutOfRange(lab)) {
      throw invalid_argument("Invalid lab when reading the data.");
    }

    copyMemory = copyDataFromNandFile();

    if (isLabDataExistInMemory(lab)) {
      string readData = copyMemory[lab];
      saveReadDataInResultFile(readData);
      return readData;
    }

     return "0x00000000";
  }

private:
  map<int, string> copyMemory;

};
