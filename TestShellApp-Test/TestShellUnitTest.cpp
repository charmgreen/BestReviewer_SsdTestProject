// Copyright 2024, Samsung
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <fstream>
#include <cstdio> // std::remove
#include "../Test_Shell_App/ShellCommandFactory.cpp"
#include "../Test_Shell_App/RealSsdDriver.cpp"
#include "../Test_Shell_App/TestShell.cpp"
#include "../Test_Shell_App/ShellCommand.cpp"

using namespace std;
using namespace testing;

class MockSsdDriver : public SsdDriver {
 public:
    MOCK_METHOD(std::string, Read, (int LBA), (override));
    MOCK_METHOD(void, Write, (int LBA, std::string Data), (override));
    MOCK_METHOD(void, Erase, (int startLBA, int Size), (override));
    MOCK_METHOD(void, Flush, (), (override));
    MOCK_METHOD(std::string, CmpBufRead, (int LBA), (override));
    int GetMinLBA() override { return 0; }
    int GetMaxLBA() override { return 99; }
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
    NiceMock<MockSsdDriver> mockSsdDriver;
    stringstream actualOutput;
    streambuf* backup_cout;
    int startOneLBA = 3;
    const int MAX_LBA_CNT = 100;
    const string UNMAPED_DATA       = "0x00000000";
    const string WRITE_DATA         = "0xAABBCCDD";
    const string INV_WRITE_DATA     = "0xAAGGCCDD";
    const string INVALID_COMMAND    = "INVALID COMMAND\n";

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
        .WillRepeatedly(Return(UNMAPED_DATA));

    testShell.Run("read " + to_string(startOneLBA));
    VerifyResult(MakeExpectedOutStr(3, UNMAPED_DATA));
}

TEST_F(MockSsdTestShellFixture, Read_FullLBA) {
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(MAX_LBA_CNT)
        .WillRepeatedly(Return(UNMAPED_DATA));

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

TEST_F(MockSsdTestShellFixture, Write_OneLBA_ArgCntFail) {
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

TEST_F(MockSsdTestShellFixture, ExitTest) {
    EXPECT_THROW(testShell.Run("exit"), ExitTestShell);
}

TEST_F(MockSsdTestShellFixture, HelpTest) {
    string expectResult = "";
    expectResult += "[Help]\n";
    expectResult += "1. write {LBA} {Data}\n";
    expectResult += "2. read {LBA}\n";
    expectResult += "3. exit\n";
    expectResult += "4. help\n";
    expectResult += "5. fullwrite {Data}\n";
    expectResult += "6. fullread\n";
    expectResult += "7. testapp1\n";
    expectResult += "8. testapp2\n";
    expectResult += "9. erase {LBA} {Size}\n";
    expectResult += "10. erase_range {LBA} {LBA}\n";
    expectResult += "11. flush\n";
    expectResult += "{LBA} = {x is an integer | 0 <= x <= 99}\n";
    expectResult += "{Data} = {""0x[0-9A-F]""}\n";
    testShell.Run("help");
    VerifyResult(expectResult);
}

TEST_F(MockSsdTestShellFixture, Erase_One) {
    EXPECT_CALL(mockSsdDriver, Erase)
        .Times(1);
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(1)
        .WillRepeatedly(Return(UNMAPED_DATA));

    testShell.Run("erase " + to_string(startOneLBA) + " 1");
    testShell.Run("read " + to_string(startOneLBA));
    VerifyResult(MakeExpectedOutStr(startOneLBA, UNMAPED_DATA));
}

TEST_F(MockSsdTestShellFixture, WriteErase_FullLBA) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(MAX_LBA_CNT);
    EXPECT_CALL(mockSsdDriver, Erase)
        .Times(1);
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(MAX_LBA_CNT)
        .WillRepeatedly(Return(UNMAPED_DATA));

    testShell.Run("fullwrite " + WRITE_DATA);
    testShell.Run("erase 0 " + to_string(MAX_LBA_CNT));
    testShell.Run("fullread");
    VerifyResult(MakeExpectedOutStr(MAX_LBA_CNT, UNMAPED_DATA));
}

TEST_F(MockSsdTestShellFixture, Erase_OOR) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(MAX_LBA_CNT);
    EXPECT_CALL(mockSsdDriver, Erase)
        .Times(1);
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(MAX_LBA_CNT)
        .WillRepeatedly(Return(UNMAPED_DATA));

    testShell.Run("fullwrite " + WRITE_DATA);
    testShell.Run("erase 0 " + to_string(MAX_LBA_CNT + MAX_LBA_CNT));
    testShell.Run("fullread");
    VerifyResult(MakeExpectedOutStr(MAX_LBA_CNT, UNMAPED_DATA));
}

TEST_F(MockSsdTestShellFixture, Erase_Partial) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(MAX_LBA_CNT);
    EXPECT_CALL(mockSsdDriver, Erase)
        .Times(1);
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(MAX_LBA_CNT)
        .WillOnce(Return(UNMAPED_DATA))
        .WillOnce(Return(UNMAPED_DATA))
        .WillOnce(Return(UNMAPED_DATA))
        .WillOnce(Return(UNMAPED_DATA))
        .WillOnce(Return(UNMAPED_DATA))
        .WillRepeatedly(Return(WRITE_DATA));

    testShell.Run("fullwrite " + WRITE_DATA);
    testShell.Run("erase 0 " + to_string(5));
    testShell.Run("fullread");

    string expectBuffer = "[FullRead]\n";
    for (int LBA = 0; LBA < 5; LBA++) {
        expectBuffer += MakeExpectedOutStr(LBA, UNMAPED_DATA);
    }
    for (int LBA = 5; LBA < MAX_LBA_CNT; LBA++) {
        expectBuffer += MakeExpectedOutStr(LBA, WRITE_DATA);
    }
    VerifyResult(expectBuffer);
}

TEST_F(MockSsdTestShellFixture, Erase_Invalid) {
    EXPECT_CALL(mockSsdDriver, Erase)
        .Times(0);

    testShell.Run("erase " + to_string(MAX_LBA_CNT) + " 1");
    VerifyResult(INVALID_COMMAND);
}


TEST_F(MockSsdTestShellFixture, EraseRange_InvalidInverted) {
    EXPECT_CALL(mockSsdDriver, Erase)
        .Times(0);

    testShell.Run("erase_range" + to_string(startOneLBA) + " " + to_string(startOneLBA - 1));
    VerifyResult(INVALID_COMMAND);
}

TEST_F(MockSsdTestShellFixture, EraseRange_OOR) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(MAX_LBA_CNT);
    EXPECT_CALL(mockSsdDriver, Erase)
        .Times(1);
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(MAX_LBA_CNT)
        .WillRepeatedly(Return(UNMAPED_DATA));

    testShell.Run("fullwrite " + WRITE_DATA);
    testShell.Run("erase_range 0 " + to_string(MAX_LBA_CNT + MAX_LBA_CNT));
    testShell.Run("fullread");
    VerifyResult(MakeExpectedOutStr(MAX_LBA_CNT, UNMAPED_DATA));
}

TEST_F(MockSsdTestShellFixture, TestApp1) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(MAX_LBA_CNT);

    EXPECT_CALL(mockSsdDriver, Read)
        .Times(MAX_LBA_CNT);

    testShell.Run("testapp1");
}

TEST_F(MockSsdTestShellFixture, TestApp2) {
    EXPECT_CALL(mockSsdDriver, Write)
        .Times(36);

    EXPECT_CALL(mockSsdDriver, Read)
        .Times(6);

    testShell.Run("testapp2");
}

TEST_F(MockSsdTestShellFixture, UnmapCompare) {
    EXPECT_CALL(mockSsdDriver, Read)
        .Times(MAX_LBA_CNT)
        .WillRepeatedly(Return(UNMAPED_DATA));
    EXPECT_CALL(mockSsdDriver, CmpBufRead)
        .Times(MAX_LBA_CNT)
        .WillRepeatedly(Return(UNMAPED_DATA));

    testShell.Run("compare");
}

#ifdef _DEBUG
// Real Ssd Driver 관련 Test Case
class RealSsdTestShellFixture : public testing::Test {
public:
    TestShell testShell;
    RealSsdDriver realSsdDriver;
protected:
    void SetUp() override {
        testShell.SetSsdDriver(&realSsdDriver);
        FormatSSD();
    }
    void TearDown() override {
    }
private:
    bool deleteFileIfExists(const string& file_path)
    {
        ifstream file(file_path);

        // 파일이 존재하는지 확인
        if (file.good()) {
            file.close(); // 파일 스트림 닫기
            if (std::remove(file_path.c_str()) == 0) {
                return true; // 파일 삭제 성공
            }
            else {
                return false; // 파일 삭제 실패
            }
        }
        else {
            return false; // 파일이 존재하지 않음
        }
    }

    void FormatSSD(void)
    {
        deleteFileIfExists("nand.txt");
        deleteFileIfExists("result.txt");
    }
};

TEST_F(RealSsdTestShellFixture, FullWriteReadCompare) {
    testShell.Run("fullwrite 0x0000000A");
    testShell.Run("fullread");
    testShell.Run("compare");
}

TEST_F(RealSsdTestShellFixture, FullRead10Compare) {
    for (int i = 0; i < 10; i++)
    {
        testShell.Run("fullread");
    }
    testShell.Run("compare");
}

TEST_F(RealSsdTestShellFixture, Loop_WriteAndReadCompare) {
    for (int i = 0; i < 8; i++)
    {
        testShell.Run("write 0 0x0000000A");
        testShell.Run("read 0");
    }
    testShell.Run("compare");
}

TEST_F(RealSsdTestShellFixture, Write10AndCompare) {
    for (int i = 0; i < 10; i++)
    {
        testShell.Run("write " + to_string(i) + " 0x0000000A");
    }
    testShell.Run("compare");
}
#endif

