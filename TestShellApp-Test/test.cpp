// Copyright [2024] <JJY>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Test_Shell_App/TestShell.cpp"

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
     const int MAX_LBA_CNT = 100;
};

TEST_F(TestShellFixture, unmap_read_1_lba) {
    EXPECT_CALL(msd, Read)
        .Times(1)
        .WillRepeatedly(Return(0));

    ts.Run("read 3");
}

TEST_F(TestShellFixture, unmap_read_full_lba) {
    EXPECT_CALL(msd, Read)
        .Times(MAX_LBA_CNT)
        .WillRepeatedly(Return(0));

    ts.Run("fullread");
}

TEST_F(TestShellFixture, write_1_lba) {
    EXPECT_CALL(msd, Write)
        .Times(1);

    ts.Run("write 3 0xAABBCCDD");
}

TEST_F(TestShellFixture, write_full_lba) {
    EXPECT_CALL(msd, Write)
        .Times(MAX_LBA_CNT);

    ts.Run("fullwrite 0xABCDFFFF");
}

TEST_F(TestShellFixture, exit) {
    ts.Run("exit");
}

TEST_F(TestShellFixture, help) {
    ts.Run("help");
}

