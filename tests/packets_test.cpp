#include "../tftp_common/packets.hpp"
#include <gtest/gtest.h>

using namespace tftp_common::packets;

#define EXPECT_DATA(buffer, base_offset, data)                                                                         \
    {                                                                                                                  \
        for (std::size_t i = 0, size = data.size(); i != size; ++i) {                                                  \
            EXPECT_EQ(buffer[base_offset + i], data[i]);                                                               \
        }                                                                                                              \
    }

#define EXPECT_STRING(buffer, base_offset, string)                                                                     \
    {                                                                                                                  \
        EXPECT_DATA(buffer, base_offset, string);                                                                      \
        EXPECT_EQ(buffer[base_offset + string.size()], 0x00);                                                          \
    }

/// Test that Request packet serialization is going fine and everything is converting to network byte order
TEST(Request, Serialization) {
    std::string filename = "example_filename.cpp";
    std::string mode = "netascii";
    auto packet = Request{Type::ReadRequest, filename, mode};

    std::vector<std::uint8_t> buffer;
    auto packetSize = packet.serialize(std::back_inserter(buffer));
    EXPECT_EQ(packetSize, sizeof(std::uint16_t) + filename.size() + mode.size() + 2);

    // type field
    // check that conversion to big-endian was done
    EXPECT_EQ(buffer[0], 0x00);
    EXPECT_EQ(buffer[1], 0x01);

    // filename field
    std::size_t base_offset = 2;
    EXPECT_STRING(buffer, base_offset, filename);

    // mode field
    base_offset += filename.size() + 1;
    EXPECT_STRING(buffer, base_offset, mode);

    EXPECT_EQ(buffer.size(), packetSize);
}

/// Test that Request packet with options serialization is going fine and everything is converting to network byte order
TEST(Request, OptionSerialization) {
    std::string filename = "example_filename.cpp";
    std::string mode = "netascii";
    std::vector<std::string> optionsNames = {
        "saveFiles", "discardQualifiers", "secret"
    };
    std::vector<std::string> optionsValues = {
        "true", "false", "Ix0e86yG8YpFzwz1gS0XxJW3"
    };
    auto packet = Request{Type::ReadRequest, filename, mode, optionsNames, optionsValues};

    std::size_t optionsSize = 0;
    for (std::size_t idx = 0; idx != optionsNames.size(); ++idx) {
        optionsSize += optionsNames[idx].size() + optionsValues[idx].size() + 2;
    }

    std::vector<std::uint8_t> buffer;
    auto packetSize = packet.serialize(std::back_inserter(buffer));
    EXPECT_EQ(packetSize, sizeof(std::uint16_t) + filename.size() + mode.size() + optionsSize + 2);

    // type field
    EXPECT_EQ(buffer[0], 0x00);
    EXPECT_EQ(buffer[1], 0x01);
    std::size_t baseOffset = 2;

    // filename field
    EXPECT_STRING(buffer, baseOffset, filename);
    baseOffset += filename.size() + 1;

    // mode field
    EXPECT_STRING(buffer, baseOffset, mode);
    baseOffset += mode.size() + 1;

    // option names and values
    for (std::size_t idx = 0; idx != optionsNames.size(); ++idx) {
        EXPECT_STRING(buffer, baseOffset, optionsNames[idx]);
        baseOffset += optionsNames[idx].size() + 1;
        EXPECT_STRING(buffer, baseOffset, optionsValues[idx]);
        baseOffset += optionsValues[idx].size() + 1;
    }

    EXPECT_EQ(buffer.size(), packetSize);
}

/// Test that Data packet serialization is going fine and everything is converting to network byte order
TEST(Data, Serialization) {
    std::vector<std::uint8_t> data;
    for (auto i = 0; i != 255; ++i) {
        data.push_back(i);
    }
    auto packet = Data{static_cast<std::uint16_t>(data.size()), data};

    std::vector<std::uint8_t> buffer;
    auto packetSize = packet.serialize(std::back_inserter(buffer));
    EXPECT_EQ(packetSize, sizeof(std::uint16_t) + sizeof(std::uint16_t) + data.size());

    // type field
    EXPECT_EQ(buffer[0], 0x00);
    EXPECT_EQ(buffer[1], 0x03);

    // block field
    EXPECT_EQ(buffer[2], 0x00);
    EXPECT_EQ(buffer[3], 0xFF);

    // data field
    EXPECT_DATA(buffer, 4, data);

    EXPECT_EQ(buffer.size(), packetSize);
}

/// Test that Acknowledgment packet serialization is going fine and everything is converting to network byte order
TEST(Acknowledgment, Serialization) {
    auto packet = Acknowledgment{255};

    std::vector<std::uint8_t> buffer;
    auto packetSize = packet.serialize(std::back_inserter(buffer));
    EXPECT_EQ(packetSize, sizeof(std::uint16_t) + sizeof(std::uint16_t));

    // type field
    EXPECT_EQ(buffer[0], 0x00);
    EXPECT_EQ(buffer[1], 0x04);

    // block field
    EXPECT_EQ(buffer[2], 0x00);
    EXPECT_EQ(buffer[3], 0xFF);

    EXPECT_EQ(buffer.size(), packetSize);
}

/// Test that Error packet serialization is going fine and everything is converting to network byte order
TEST(Error, Serialization) {
    std::string errorMessage = "Something went wrong...";
    auto packet = Error{0x01, errorMessage};

    std::vector<std::uint8_t> buffer;
    auto packetSize = packet.serialize(std::back_inserter(buffer));
    EXPECT_EQ(packetSize, sizeof(std::uint16_t) + sizeof(std::uint16_t) + errorMessage.size() + 1);

    // type field
    EXPECT_EQ(buffer[0], 0x00);
    EXPECT_EQ(buffer[1], 0x05);

    // errorCode field
    EXPECT_EQ(buffer[2], 0x00);
    EXPECT_EQ(buffer[3], 0x01);

    // errorMessage field
    EXPECT_DATA(buffer, 4, errorMessage);

    EXPECT_EQ(buffer.size(), packetSize);
}

/// Test that Option Acknowledgment packet serialization is going fine and everything is converting to network byte order
TEST(OptionAcknowledgment, Serialization) {
    std::vector<std::string> optionsNames = {
        "saveFiles", "discardQualifiers", "secret"
    };
    std::vector<std::string> optionsValues = {
        "true", "false", "Ix0e86yG8YpFzwz1gS0XxJW3"
    };
    auto packet = OptionAcknowledgment { optionsNames, optionsValues };

    std::size_t optionsSize = 0;
    for (std::size_t idx = 0; idx != optionsNames.size(); ++idx) {
        optionsSize += optionsNames[idx].size() + optionsValues[idx].size() + 2;
    }

    std::vector<std::uint8_t> buffer;
    auto packetSize = packet.serialize(std::back_inserter(buffer));
    EXPECT_EQ(packetSize, sizeof(std::uint16_t) + optionsSize);

    std::size_t baseOffset = sizeof(std::uint16_t);
    // option names and values
    for (std::size_t idx = 0; idx != optionsNames.size(); ++idx) {
        EXPECT_STRING(buffer, baseOffset, optionsNames[idx]);
        baseOffset += optionsNames[idx].size() + 1;
        EXPECT_STRING(buffer, baseOffset, optionsValues[idx]);
        baseOffset += optionsValues[idx].size() + 1;
    }

    EXPECT_EQ(buffer.size(), packetSize);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}