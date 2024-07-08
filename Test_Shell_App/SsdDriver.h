// Copyright [2024] <CRA/BestReviewer>
#pragma once

class SsdDriver {
 public:
    virtual unsigned int Read(int LBA) = 0;
    virtual void Write(int LBA, unsigned int Data) = 0;
};
