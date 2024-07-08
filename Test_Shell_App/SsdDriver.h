#pragma once

class SsdDriver
{
public:
    virtual int Read(int LBA) = 0;
    virtual void Write(int LBA, int Data) = 0;
};