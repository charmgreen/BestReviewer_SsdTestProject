#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Test_Shell_App/SsdDriver.h"
#include "../Test_Shell_App/TestShell.cpp"

using namespace std;
using namespace testing;

class MockSsdDriver : public SsdDriver
{
public:
    //MOCK_METHOD(void, write, (int addr, int value), (override));
    //MOCK_METHOD(int, read, (int addr), (override));
};

class TestShellFixture : public testing::Test
{
protected:
    void SetUp() override
    {
    }
public:
};

TEST_F(TestShellFixture, unmap_read)
{
    EXPECT_EQ(0, 0);
}

TEST_F(TestShellFixture, unmap_read_oor)
{
    EXPECT_EQ(0, 0);
}