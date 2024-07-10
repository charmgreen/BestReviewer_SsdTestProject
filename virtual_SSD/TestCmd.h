// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <vector>
#include <string>
#include "SSD.h"
#include "Parser.h"
#include "SSDInterface.h"


class TestCmd {
 public:
    TestCmd(SSDInterface *ssd_i) 
        : ssd(ssd_i), parser(nullptr), cmd(nullptr){
    };

    void Run(const std::string& str);
    Parser* SetParser();
    SSDInterface* SetSSD();

 protected:
    SSDInterface *ssd;

 private:
    Parser* parser;
    CmdStatus* cmd;
    
    std::string _Read();
    void _Write();
};

