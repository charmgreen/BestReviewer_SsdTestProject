#pragma once
#include<map>
#include<string>

class SSD{
public:
  virtual std::string Read(int lab) = 0;

  std::map<int, std::string> getMemory() {
	  return memory;
  }

private:
  std::map<int, std::string> memory;
};
