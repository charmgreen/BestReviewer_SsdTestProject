// Copyright [2024] <JJY>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Test_Shell_App/SsdDriver.h"
#include "../Test_Shell_App/TestShell.h"

using namespace std;
using namespace testing;

class MockSsdDriver : public SsdDriver {
public:
    MOCK_METHOD(int, Read, (int LBA), (override));
    MOCK_METHOD(void, Write, (int LBA, int Data), (override));
};

class TestShellFixture : public testing::Test {
 protected:
    void SetUp() override {
    }

 public:
     TestShell ts;
     MockSsdDriver msd;
};

TEST_F(TestShellFixture, unmap_read_1_lba) {
    EXPECT_CALL(msd, Read)
        .Times(1)
        .WillRepeatedly(Return(0));

    ts.Run("read 3");
}

TEST_F(TestShellFixture, unmap_full_read) {
    EXPECT_CALL(msd, Read)
        .Times(100)
        .WillRepeatedly(Return(0));

    ts.Run("fullread");
}
