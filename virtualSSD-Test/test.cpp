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
//
//// LBA 범위가 아닌 경우 exception throw test
//TEST(TestCaseForVirtualSSD, excpetionWhenInvalidLBA) { 
//	VirtualSSD ssd; 
//}
