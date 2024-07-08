#pragma once

class TestShell {
 public:
    void Run();

 private:
    bool getUserInput(char Input[]);
    void parseArgument();
    static const int MAX_INPUT_LENGTH = 50;
    char RawInput[MAX_INPUT_LENGTH];

    enum class ShellCommand {
        Write,
        Read,
    };
};
