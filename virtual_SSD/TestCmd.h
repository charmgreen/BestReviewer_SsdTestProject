#pragma once
// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <vector>
#include <string>
#include "SSD.h"
#include "Parser.h"

class TestCmd {
public:
    void Run(const std::string& str);
    SSD* SetSSD();
    Parser* SetParser();

private:
    Parser* parser;
    SSD* ssd;
    CmdStatus* cmd;
    
    std::string _Read();
    void _Write();
};

