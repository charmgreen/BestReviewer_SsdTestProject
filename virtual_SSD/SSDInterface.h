#pragma once
#define interface struct

interface SSDInterface {
  virtual void Write(const int &LBA, const std::string &data) = 0;
  virtual std::string Read(const int &LBA) = 0;
};
