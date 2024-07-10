// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <vector>
#include <string>
#include "ShellCommand.h"
#include "ShellCommandFactory.h"

class ShellScript : public ShellCommand, public ShellCommandFactory {
 public:
    ShellScript(std::string filename, SsdDriver* ssddriver);
    void Run(SsdDriver* ssddriver) override;
 private:
    std::string filename;
};
