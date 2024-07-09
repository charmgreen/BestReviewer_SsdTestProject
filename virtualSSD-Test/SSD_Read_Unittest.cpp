#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../virtual_SSD/SSD.cpp"

class SSDFixture_R : public testing::Test {
public:
  SSD ssd_R;

private:

};

TEST_F(SSDFixture_R, readDataTest) { EXPECT_THAT("0x10000001", ssd_R.Read(0)); }

TEST_F(SSDFixture_R, readDataWhereEmptyLBATest) {
  EXPECT_THAT("0x00000000", ssd_R.Read(1));
}

TEST_F(SSDFixture_R, exceptionThrowMinusLBAWhenReadData) {
  EXPECT_THROW(ssd_R.Read(-10), LBARangeException);
}

// TEST(TestCaseForVirtualSSD, exceptionThrow100LBAWhenReadData) {
//   SSD ssd;
//   EXPECT_THROW({ ssd.Read(100); }, std::invalid_argument);
// }