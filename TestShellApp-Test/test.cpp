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
     MockSsdDriver msd;
};

TEST_F(TestShellFixture, unmap_read) {
    EXPECT_CALL(msd, Read)
        .WillRepeatedly(Return(0));

    int data = msd.Read(0);
    EXPECT_EQ(data, 0);
}

TEST_F(TestShellFixture, unmap_full_read) {
    EXPECT_CALL(msd, Read)
        .WillRepeatedly(Return(0));

    for (int i = 0; i < 100; i++)
    {
        int data = msd.Read(i);
        EXPECT_EQ(data, 0);
    }
}
