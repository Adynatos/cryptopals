#include <gtest/gtest.h>
#include <string>
#include "helpers.hpp"


TEST(TestHelpers, ShouldConvertHexStringToByteString)
{
    std::string input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    std::string result = "I'm killing your brain like a poisonous mushroom";
    ASSERT_EQ(result, hexToByte(input));
}

TEST(TestHelpers, ShouldConvertSingleByteToHexString)
{
    std::string input = "*";
    std::string result = "2a";
    ASSERT_EQ(result, byteToHex(input));
}

TEST(TestHelpers, ShouldConvertByteToHexStringWithPadding)
{
    ASSERT_EQ("0a", byteToHex(hexToByte("0a")));
}

TEST(TestHelpers, ShouldConvertByteStringToHexString)
{
    std::string input = "I'm killing your brain like a poisonous mushroom";
    std::string result = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    ASSERT_EQ(result, byteToHex(input));
}

TEST(TestHelpers, ShouldXorByteStrings)
{
    std::string first = hexToByte("1c0111001f010100061a024b53535009181c");
    std::string second = hexToByte("686974207468652062756c6c277320657965");
    std::string result = "746865206b696420646f6e277420706c6179";
    ASSERT_EQ(result, byteToHex(fixedXor(first, second)));
}

TEST(TestHelpers, ShouldXorWithSingleByte)
{
    std::string input = "abcdefgh";
    ASSERT_EQ("0003020504070609", byteToHex(singleXor(input, 'a')));
}
