//  Copyright [2024] <CRA/BestReviewer>
#pragma once
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../virtual_SSD/SSD.cpp"
#include "../virtual_SSD/TestCmd.cpp"
#include "../virtual_SSD/Parser.cpp"

using namespace testing;

class MockSSD : public SSDInterface {
 public:
    MOCK_METHOD(void, Write, (const int &LBA, const std::string &data), (override));
    MOCK_METHOD(std::string, Read, (const int &LBA), (override));
};

class SSDFixture : public testing::Test {
public:

    MockSSD mockSSD;
    TestCmd testCmd{&mockSSD}; 

    SSD ssd;
    //TestCmd testMock;

    std::string getLSBData(int LBA) {
    std::string line;
    std::ifstream writeFIle("nand.txt");

    if (writeFIle.is_open()) {
      for (int i = 0; i < LBA; i++) {
        if (!std::getline(writeFIle, line)) {
          break;
        }
      }
      getline(writeFIle, line);
      int LBADataFIrstIndex = line.find(" ");
      writeFIle.close();
      return line.substr(LBADataFIrstIndex + 1);
    }
    return "0x00000000";
  }
};

TEST_F(SSDFixture, TestReadCommand) { 
    EXPECT_CALL(mockSSD, Read(0)).Times(1);
    //EXPECT_CALL(mockSSD, Read(0))
    //    .WillOnce(Return("0x10000002"));
    testCmd.Run("R 0");
    //std::cout << testCmd.Run("R 0") << std::endl;
}

TEST_F(SSDFixture, TestLBARangeExceptionWhenWrite) {
    EXPECT_THROW(ssd.Write(100, "0x10000000"), LBARangeException);
}

TEST_F(SSDFixture, TestDataRangeException) {
    EXPECT_THROW(ssd.Write(99, "0x100000"), DataRangeException);
}

TEST_F(SSDFixture, TestWriteMemory) {
    ssd.Write(0, "0x10000001");
    EXPECT_EQ("0x10000001", getLSBData(0));
    ssd.Write(3, "0x10000099");
    EXPECT_EQ("0x10000099", getLSBData(3));
}

TEST_F(SSDFixture, TestReadMemory) { 
    EXPECT_THAT("0x10000001", ssd.Read(0)); 
}

TEST_F(SSDFixture, TestReadMemoryWhenEmpty) {
    EXPECT_THAT("0x00000000", ssd.Read(1));
}

TEST_F(SSDFixture, TestMinusLBARangeExceptionWhenRead) {
    EXPECT_THROW(ssd.Read(-10), LBARangeException);
}

TEST_F(SSDFixture, Test100LBARangeExceptionWhenRead) {
    EXPECT_THROW(ssd.Read(100), LBARangeException);
}