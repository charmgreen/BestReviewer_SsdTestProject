// Copyright [2024] <JJY>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Test_Shell_App/ShellCommandFactory.cpp"
#include "../Test_Shell_App/RealSsdDriver.cpp"
#include "../Test_Shell_App/TestShell.cpp"

using namespace std;
using namespace testing;

class MockSsdDriver : public SsdDriver {
 public:
    MOCK_METHOD(std::string, Read, (int LBA), (override));
    MOCK_METHOD(void, Write, (int LBA, std::string Data), (override));
};

class MockSsdTestShellFixture : public testing::Test {
 protected:
    void SetUp() override {
        backup_cout = cout.rdbuf(actualOutput.rdbuf());
        testShell.SetSsdDriver(&mockSsdDriver);
    }
    void TearDown() override {
        cout.rdbuf(backup_cout);
    }
 public:
    TestShell testShell;
    MockSsdDriver mockSsdDriver;
    stringstream actualOutput;
    streambuf* backup_cout;
    const int MAX_LBA_CNT = 100;
    const int UNMAPED_DATA = 0x00000000;

    string toHexString(unsigned int value) {
        stringstream stream;
        stream << uppercase << setw(8) << setfill('0') << hex << value;
        return stream.str();
    }

    string MakeExpectedOutStr(int LBA, unsigned int Data) {
        string expectedOutStr;
        if (LBA == MAX_LBA_CNT) {
            expectedOutStr = "[FullRead]\n";
            for (int i = 0; i < MAX_LBA_CNT; i++) {
                expectedOutStr += ("[Read] LBA : " + to_string(i));
                expectedOutStr += (", Data : 0x" + toHexString(Data) + "\n");
            }
        }
        else {
            expectedOutStr = "[Read] LBA : " + to_string(3) + ", Data : 0x" + toHexString(Data) + "\n";
        }

        return expectedOutStr;
    }

    void VerifyResult(string expectedOutStr) {
        EXPECT_THAT(actualOutput.str(), StrEq(expectedOutStr));
    }
};

TEST_F(MockSsdTestShellFixture, unmap_read_1_lba) {
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(1)
        .WillRepeatedly(Return("0x00000000"));

    testShell.Run("read 3");
    VerifyResult(MakeExpectedOutStr(3, UNMAPED_DATA));
}

TEST_F(MockSsdTestShellFixture, unmap_read_1_invalid_lba) {
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(0);

    testShell.Run("read " + to_string(MAX_LBA_CNT));
    VerifyResult("INVALID COMMAND\n");
}

TEST_F(MockSsdTestShellFixture, unmap_read_full_lba) {
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(MAX_LBA_CNT)
        .WillRepeatedly(Return("0x00000000"));

    testShell.Run("fullread");
    VerifyResult(MakeExpectedOutStr(MAX_LBA_CNT, UNMAPED_DATA));
}

TEST_F(MockSsdTestShellFixture, write_1_lba) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(1);

    testShell.Run("write 3 0xAABBCCDD");
}

TEST_F(MockSsdTestShellFixture, write_1_invalid_lba) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(0);

    testShell.Run("write " + to_string(MAX_LBA_CNT) + " 0xAABBCCDD");
    VerifyResult("INVALID COMMAND\n");
}

TEST_F(MockSsdTestShellFixture, write_1_invalid_data) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(0);

    testShell.Run("write 99 0xAAKKCCDD");
    VerifyResult("INVALID COMMAND\n");
}

TEST_F(MockSsdTestShellFixture, write_full_lba) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(MAX_LBA_CNT);

    testShell.Run("fullwrite 0xABCDFFFF");
}

TEST_F(MockSsdTestShellFixture, invalid_command) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(0);

    testShell.Run("fullrite 0xABCDFFFF");
    VerifyResult("INVALID COMMAND\n");
}

TEST_F(MockSsdTestShellFixture, invalid_argument_cnt) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(0);

    testShell.Run("write 3 0xAABBCCDD test");
    VerifyResult("INVALID COMMAND\n");
}

TEST_F(MockSsdTestShellFixture, invalid_argument_cnt2) {
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(0);

    testShell.Run("Read 3 0xAABBCCDD test");
    VerifyResult("INVALID COMMAND\n");
}

TEST_F(MockSsdTestShellFixture, invalid_argument_cnt3) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(0);

    testShell.Run("fullwrite 0xABCDFFFF \n fullread");
}

TEST_F(MockSsdTestShellFixture, invalid_argument_cnt4) {
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(0);

    testShell.Run("fullread 0xABCDFFFF \n fullread");
}

TEST_F(MockSsdTestShellFixture, exit) {
    testShell.Run("exit");
    VerifyResult("[Exit] Quit Shell\n");
}

TEST_F(MockSsdTestShellFixture, help) {
    testShell.Run("help");
    VerifyResult("[Help]\n");
}

TEST_F(MockSsdTestShellFixture, help_after_exit) {
    testShell.Run("exit");
    testShell.Run("help");
    VerifyResult("[Exit] Quit Shell\n");
}

// Real Ssd Driver 관련 Test Case 는 이후 따로 추가 예정
TEST(RealSsdTestShellFixture, real_unmap_read_1_lba) {
    TestShell testShell2;
    RealSsdDriver realSsdDriver;
    testShell2.SetSsdDriver(&realSsdDriver);
    testShell2.Run("read 3");
}
