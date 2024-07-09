// Copyright 2024, Samsung
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Test_Shell_App/ShellCommandFactory.cpp"
#include "../Test_Shell_App/RealSsdDriver.cpp"
#include "../Test_Shell_App/TestShell.cpp"
#include "../Test_Shell_App/ShellCommand.cpp"
#include "../Test_Shell_App/ShellScript.cpp"

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
    int startOneLBA = 3;
    const int MAX_LBA_CNT = 100;
    const string UNMAPED_DATA = "0x00000000";
    const string WRITE_DATA = "0xAABBCCDD";
    const string INV_WRITE_DATA = "0xAAGGCCDD";
    const string INVALID_COMMAND = "INVALID COMMAND\n";

    string MakeExpectedOutStr(int LBA, string Data) {
        string expectedOutStr;
        if (LBA == MAX_LBA_CNT) {
            expectedOutStr = "[FullRead]\n";
            for (int i = 0; i < MAX_LBA_CNT; i++) {
                expectedOutStr += ("[Read] LBA : " + to_string(i));
                expectedOutStr += (", Data : " + Data + "\n");
            }
        }
        else {
            expectedOutStr = "[Read] LBA : " + to_string(LBA) + ", Data : " + Data + "\n";
        }

        return expectedOutStr;
    }

    void VerifyResult(string expectedOutStr) {
        EXPECT_THAT(actualOutput.str(), StrEq(expectedOutStr));
    }
};

TEST_F(MockSsdTestShellFixture, Read_OneLBA) {
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(1)
        .WillRepeatedly(Return("0x00000000"));

    testShell.Run("read " + to_string(startOneLBA));
    VerifyResult(MakeExpectedOutStr(3, UNMAPED_DATA));
}

TEST_F(MockSsdTestShellFixture, Read_FullLBA) {
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(MAX_LBA_CNT)
        .WillRepeatedly(Return("0x00000000"));

    testShell.Run("fullread");
    VerifyResult(MakeExpectedOutStr(MAX_LBA_CNT, UNMAPED_DATA));
}

TEST_F(MockSsdTestShellFixture, WriteAndRead_OneLBA) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(1);
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(1)
        .WillRepeatedly(Return(WRITE_DATA));

    testShell.Run("write " + to_string(startOneLBA) + " " + WRITE_DATA);
    testShell.Run("read " + to_string(startOneLBA));
    VerifyResult(MakeExpectedOutStr(startOneLBA, WRITE_DATA));
}

TEST_F(MockSsdTestShellFixture, WriteAndRead_FullLBA) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(MAX_LBA_CNT);
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(MAX_LBA_CNT)
        .WillRepeatedly(Return(WRITE_DATA));

    testShell.Run("fullwrite " + WRITE_DATA);
    testShell.Run("fullread");
    VerifyResult(MakeExpectedOutStr(MAX_LBA_CNT, WRITE_DATA));
}

TEST_F(MockSsdTestShellFixture, Read_OneLBA_OOR) { // out of range
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(0);

    testShell.Run("read " + to_string(MAX_LBA_CNT));
    VerifyResult(INVALID_COMMAND);
}

TEST_F(MockSsdTestShellFixture, Write_OneLBA_OOR) { // out of range
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(0);

    testShell.Run("write " + to_string(MAX_LBA_CNT) + WRITE_DATA);
    VerifyResult(INVALID_COMMAND);
}

TEST_F(MockSsdTestShellFixture, invalid_argument_cnt) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(0);

    testShell.Run("write " + to_string(startOneLBA) + " " + WRITE_DATA + " test");
    VerifyResult(INVALID_COMMAND);
}

TEST_F(MockSsdTestShellFixture, Read_OneLBA_ArgCntFail) {
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(0);

    testShell.Run("read " + to_string(startOneLBA) + " " + WRITE_DATA);
    VerifyResult(INVALID_COMMAND);
}

TEST_F(MockSsdTestShellFixture, Write_OneLBA_InvWriteData) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(0);

    testShell.Run("write " + to_string(startOneLBA) + " " + INV_WRITE_DATA);
    VerifyResult(INVALID_COMMAND);
}

TEST_F(MockSsdTestShellFixture, InvCommand) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(0);

    testShell.Run("fullrite " + to_string(startOneLBA));
    VerifyResult(INVALID_COMMAND);
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

TEST_F(MockSsdTestShellFixture, testapp1) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(MAX_LBA_CNT);

    EXPECT_CALL(mockSsdDriver, Read)
        .Times(MAX_LBA_CNT);

    testShell.Run("testapp1");
}

TEST_F(MockSsdTestShellFixture, testapp2) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(36);

    EXPECT_CALL(mockSsdDriver, Read)
        .Times(6);

    testShell.Run("testapp2");
}

//TEST(RealSsdTestShellFixture, Read_OneLBA) {
//    TestShell testShell2;
//    RealSsdDriver realSsdDriver;
//    testShell2.SetSsdDriver(&realSsdDriver);
//    testShell2.Run("read 3");
//}

