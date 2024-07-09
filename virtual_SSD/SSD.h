#pragma once
#include<map>
#include<string>

using namespace std;

class SSD{
public:
  virtual string Read(int lba);
  virtual void saveReadDataInResultFile(string readData);
  virtual map<int, string> copyDataFromNandFile();
  virtual bool isLbaOutOfRange(int lba);

  map<int, string> getMemory() {
	  return memory;
  }

private:
  map<int, string> memory;
};
