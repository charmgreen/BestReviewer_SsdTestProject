#include "SSD.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

class VirtualSSD : SSD {
public:
  string SSD::Read(int lab) override {
    // if (lab < 0 || lab > 99)
    //   thorwInvalidException();

    ifstream nandFile("nand.txt");

    map<int, string> tmpMemory;

    if (nandFile.is_open()) {
      string line;
      int tmplba = 0;
      while (!nandFile.eof()) {
        getline(nandFile, line);
        tmpMemory.insert({tmplba, line});
        tmplba++;
      }
      setMemory(tmpMemory);

      nandFile.close();
    } else {
      return "0x00000000";
    }

    if (tmpMemory.find(lab) != tmpMemory.end()) {
      ofstream resultFile("result.txt");
      resultFile << tmpMemory[lab];
      resultFile.close();

      return tmpMemory[lab];
    } else {
      return "0x00000000";
    }
  }

};
