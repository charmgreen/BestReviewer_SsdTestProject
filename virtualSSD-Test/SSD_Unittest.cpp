//  Copyright [2024] <CRA/BestReviewer>
#pragma once
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../virtual_SSD/SSD.cpp"
#include "../virtual_SSD/SSDCommand.cpp"
#include "../virtual_SSD/Parser.cpp"

using namespace testing;

class MockSSD : public SSDInterface {
 public:
    MOCK_METHOD(void, Write, (const int &LBA, const std::string &data), (override));
    MOCK_METHOD(void, Read, (const int &LBA), (override));
    MOCK_METHOD(void, Erase, (const int &LBA, const int &size), (override));
};

class SSDFixture : public testing::Test {
 public:
    NiceMock<MockSSD> mockSSD;
    Parser parser;
    CmdStatus cmd;
    SSDCommand testCmd{&mockSSD, &parser, &cmd};

    SSD ssd;
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

TEST_F(SSDFixture, TestLBARangeExceptionWhenWrite) {
    EXPECT_THROW(ssd.Write(100, "0x10000000"), LBARangeException);
}

TEST_F(SSDFixture, TestDataRangeException) {
    EXPECT_THROW(ssd.Write(99, "0x100000"), DataRangeException);
}

TEST_F(SSDFixture, TestDataPreFIxException) {
    EXPECT_THROW(ssd.Write(99, "0110000000"), DataPreFIxException);
}

TEST_F(SSDFixture, TestDataTypeException) {
    EXPECT_THROW(ssd.Write(99, "0x1000000Z"), DataTypeException);
}

TEST_F(SSDFixture, TestWriteMemory) {
    ssd.Write(0, "0x10000001");
    EXPECT_EQ("0x10000001", getLSBData(0));
    ssd.Write(98, "0x10000099");
    EXPECT_EQ("0x10000099", getLSBData(98));
}

TEST_F(SSDFixture, TestWriteCommandWithMock) {
  EXPECT_CALL(mockSSD, Write).Times(1);
  testCmd.Run("SSD.exe W 0 0x00000001");
}

TEST_F(SSDFixture, TestLBARangeExceptionWhenRead) {
    EXPECT_THROW(ssd.Read(-10), LBARangeException);
    EXPECT_THROW(ssd.Read(100), LBARangeException);
}

TEST_F(SSDFixture, TestReadCommandWithMock) {
    EXPECT_CALL(mockSSD, Read).Times(1);
    testCmd.Run("SSD.exe R 0");
}

TEST_F(SSDFixture, TestLBARangeExceptionWhenErase) {
    EXPECT_THROW(ssd.Erase(-10, 10), LBARangeException);
    EXPECT_THROW(ssd.Erase(100, 10), LBARangeException);
}

TEST_F(SSDFixture, TestEraseSizeRangeException) {
    EXPECT_THROW(ssd.Erase(1, 11), EraseSizeException);
    EXPECT_THROW(ssd.Erase(1, -1), EraseSizeException);
}

TEST_F(SSDFixture, TestEraseMemory) {
    ssd.Erase(0, 1);
    EXPECT_EQ("0x00000000", getLSBData(0));
}

TEST_F(SSDFixture, TestEraseMemoryWithMaxLBA) {
    ssd.Erase(98, 10);
    EXPECT_EQ("0x00000000", getLSBData(98));
    EXPECT_EQ("0x00000000", getLSBData(99));
}

TEST_F(SSDFixture, TestEraseCommandWithMock) {
  EXPECT_CALL(mockSSD, Erase).Times(1);
  testCmd.Run("SSD.exe E 0 0x00000001");
}
