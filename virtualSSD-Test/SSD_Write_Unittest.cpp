// "Copyright [2024] <doyun kim>"

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../virtual_SSD/SSD.cpp"

class SSDFixture : public testing::Test {
 public:
    SSD ssd;
    std::string getLSBData(int LBA) {
        std::string line;
        std::ifstream writeFIle("nand.txt");

        if (writeFIle.is_open()) {
            for (int i = 0; i < LBA; i++) {
                if (!std::getline(writeFIle, line)) {
                    break;
                }
            }
            getline(writeFIle, line);
            int LBADataFIrstIndex = line.find(" ");
            writeFIle.close();
            return line.substr(LBADataFIrstIndex + 1);
        }
        return "0x00000000";
    }
};

TEST_F(SSDFixture, TestLBARangeException) {
    EXPECT_THROW(ssd.Write(100, "0x10000000"), LBARangeException);
}

TEST_F(SSDFixture, TestDataRangeException) {
    EXPECT_THROW(ssd.Write(99, "0x100000"), DataRangeException);
}

TEST_F(SSDFixture, TestWriteMemory) {
    ssd.Write(0, "0x10000001");
    EXPECT_EQ("0x10000001", getLSBData(0));
    ssd.Write(3, "0x10000099");
    EXPECT_EQ("0x10000099", getLSBData(3));
}
