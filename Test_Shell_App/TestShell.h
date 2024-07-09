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
    ShellCommandFactory factory;
    ShellCommand* shellCommand;

    void _InvalidCommand();
    void _Write();
    void _Read();
    void _Exit();
    void _Help();
    void _FullWrite();
    void _FullRead();
    void _TestApp1();
    void _TestApp2();
};
