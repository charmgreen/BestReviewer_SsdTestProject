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

  map<int, string> copyDataFromNandFile(ifstream &nandFile) {
    map<int, string> copyMemory2;
    string readLine;
    int labNum = 0;
    while (!nandFile.eof()) {
      getline(nandFile, readLine);
      copyMemory2.insert({labNum, readLine});
      labNum++;
    }
    return copyMemory2;
  }

  bool isLabDataExistInMemory(int lab) {
    return copyMemory.find(lab) != copyMemory.end();
  }

  string SSD::Read(int lab) override {
    if (lab < 0 || lab > 99) {
      throw invalid_argument("Invalid lab when reading the data.");
    }

    ifstream nandFile("nand.txt");
    if (!nandFile.is_open()) {
      return "0x00000000";
    }

    copyMemory = copyDataFromNandFile(nandFile);
    nandFile.close();

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
