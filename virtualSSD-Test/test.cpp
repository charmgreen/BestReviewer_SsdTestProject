#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../virtual_SSD/SSD.h"
#include "../virtual_SSD/SSD.cpp"

using namespace testing;


TEST(TestCaseForVirtualSSD, readDataTest) {
  VirtualSSD ssd;
  EXPECT_THAT("0x00000001", ssd.Read(0));
}

TEST(TestCaseForVirtualSSD, readDataWhereEmptyLBATest) {
  VirtualSSD ssd;
  EXPECT_THAT("0x00000000", ssd.Read(3));
}

TEST(TestCaseForVirtualSSD, exceptionThrowMinusLBAWhenReadData) {
  VirtualSSD ssd;
  EXPECT_THROW({ ssd.Read(-10); }, std::invalid_argument);
}

TEST(TestCaseForVirtualSSD, exceptionThrow100LBAWhenReadData) {
  VirtualSSD ssd;
  EXPECT_THROW({ ssd.Read(100); }, std::invalid_argument);
}
