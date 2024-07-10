// Copyright [2024] <CRA/BestReviewer>
#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include "SsdDriver.h"

class ShellCommand {
 public:
     virtual void Run(SsdDriver* ssddriver) = 0;
 protected:
     const int MAX_LBA = 99;
     const int MIN_LBA = 0;
};


class InvalidCommand : public ShellCommand {
 public:
    InvalidCommand() {}
    void Run(SsdDriver* ssddriver) override;
};

class WriteCommand : public ShellCommand {
 public:
    WriteCommand(std::string strLBA, std::string strData);
    void Run(SsdDriver* ssddriver) override;
 private:
    int nLBA;
    std::string strLBA;
    std::string strData;
};

class ReadCommand : public ShellCommand {
 public:
    explicit ReadCommand(std::string strLBA);
    void Run(SsdDriver* ssddriver) override;
 private:
    int nLBA;
    std::string strLBA;
};

class ExitCommand : public ShellCommand {
 public:
    ExitCommand() {}
    void Run(SsdDriver* ssddriver) override;
};

class HelpCommand : public ShellCommand {
 public:
    HelpCommand() {}
    void Run(SsdDriver* ssddriver) override;
};

class FullWriteCommand : public ShellCommand {
 public:
    explicit FullWriteCommand(std::string strData);
    void Run(SsdDriver* ssddriver) override;
 private:
    std::string strData;
};

class FullReadCommand : public ShellCommand {
 public:
    FullReadCommand() {}
    void Run(SsdDriver* ssddriver) override;
};

class TestApp1 : public ShellCommand {
 public:
    TestApp1() {}
    void Run(SsdDriver* ssddriver) override;
};

class TestApp2 : public ShellCommand {
 public:
    TestApp2() {}
    void Run(SsdDriver* ssddriver) override;
};

class ExitTestShell : public std::exception {
 public:
    char const* what() const override {
        return "Exit Test Shell!";
    }
};
