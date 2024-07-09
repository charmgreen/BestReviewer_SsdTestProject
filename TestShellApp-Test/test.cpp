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

class TestShellFixture : public testing::Test {
 protected:
    void SetUp() override {
        backup_cout = cout.rdbuf(outBuffer.rdbuf());
        ts.SetSsdDriver(&msd);
    }
    void TearDown() override {
        cout.rdbuf(backup_cout);
    }
 public:
     TestShell ts;
     MockSsdDriver msd;
     RealSsdDriver rsd;
     const int MAX_LBA_CNT = 100;

    stringstream outBuffer;
    streambuf* backup_cout;

    void VerifyOutBuffer(string expectOut) {
        EXPECT_THAT(outBuffer.str(), StrEq(expectOut));
    }
};

TEST_F(TestShellFixture, unmap_read_1_lba) {
    EXPECT_CALL(msd, Read)
        .Times(1)
        .WillRepeatedly(Return("0x00000000"));

    ts.Run("read 3");
    VerifyOutBuffer("[Read] LBA : 3, Data : 0x00000000\n");
}

TEST_F(TestShellFixture, real_unmap_read_1_lba) {
    ts.SetSsdDriver(&rsd);
    ts.Run("read 3");
}

TEST_F(TestShellFixture, unmap_read_1_invalid_lba) {
    EXPECT_CALL(msd, Read)
        .Times(0);

    ts.Run("read 100");
    VerifyOutBuffer("INVALID COMMAND\n");
}

TEST_F(TestShellFixture, unmap_read_full_lba) {
    string expectOut = "[FullRead]\n";
    for (int LBA = 0; LBA < MAX_LBA_CNT; LBA++) {
        expectOut += ("[Read] LBA : " + to_string(LBA));
        expectOut += (", Data : 0x00000000\n");
    }
    EXPECT_CALL(msd, Read)
        .Times(MAX_LBA_CNT)
        .WillRepeatedly(Return("0x00000000"));

    ts.Run("fullread");
    VerifyOutBuffer(expectOut);
}

TEST_F(TestShellFixture, write_1_lba) {
    EXPECT_CALL(msd, Write)
        .Times(1);

    ts.Run("write 3 0xAABBCCDD");
    VerifyOutBuffer("[Write] LBA : 3, Data : 0xAABBCCDD\n");
}

TEST_F(TestShellFixture, write_1_invalid_lba) {
    EXPECT_CALL(msd, Write)
        .Times(0);

    ts.Run("write 100 0xAABBCCDD");
    VerifyOutBuffer("INVALID COMMAND\n");
}

TEST_F(TestShellFixture, write_1_invalid_data) {
    EXPECT_CALL(msd, Write)
        .Times(0);

    ts.Run("write 99 0xAAKKCCDD");
    VerifyOutBuffer("INVALID COMMAND\n");
}

TEST_F(TestShellFixture, write_full_lba) {
    EXPECT_CALL(msd, Write)
        .Times(MAX_LBA_CNT);

    ts.Run("fullwrite 0xABCDFFFF");
    string expectOut = "[FullWrite]\n";
    for (int LBA = 0; LBA < MAX_LBA_CNT; LBA++) {
        expectOut += ("[Write] LBA : " + to_string(LBA));
        expectOut += (", Data : 0xABCDFFFF\n");
    }
}

TEST_F(TestShellFixture, invalid_command) {
    ts.Run("fullrite 0xABCDFFFF");
    VerifyOutBuffer("INVALID COMMAND\n");
}

TEST_F(TestShellFixture, exit) {
    ts.Run("exit");
    VerifyOutBuffer("[Exit] Quit Shell\n");
}

TEST_F(TestShellFixture, help) {
    ts.Run("help");
    VerifyOutBuffer("[Help]\n");
}

