#pragma once
#include<map>
#include<string>

class SSD{
public:
  virtual std::string Read(int lab) = 0;

  std::map<int, std::string> getMemory() {
	  return memory;
  }

  void setMemory(std::map<int, std::string> newMemory) {
	  memory = newMemory;
  }

private:
  std::map<int, std::string> memory;
};
