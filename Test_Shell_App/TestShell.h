// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include "SsdDriver.h"
#include "ShellCommandFactory.h"


class TestShell {
 public:
    void Run(const std::string& str);
    void SetSsdDriver(SsdDriver* ssddriver);

 private:
    SsdDriver* ssddriver;
    ShellCommandFactory shellCommandFactory;
    ShellCommand* shellCommand;
};
