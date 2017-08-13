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

TEST(TestHelpers, ShouldDecodeSingleXor)
{
    std::string message = "Force and fraud are in war the two cardinal virtues";
    std::string encoded = singleXor(message, 'F');
    ASSERT_EQ(message, decodeSingleXor(encoded));
}

TEST(TestHelpers, ShouldXorWithRepeatedKey)
{
    std::string message = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    std::string expected = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

    ASSERT_EQ(expected, byteToHex(repeatingXor(message, "ICE")));
}

TEST(TestHelpers, ShouldCalculateHammingDistance)
{
    std::string first = "this is a test";
    std::string second = "wokka wokka!!!";
    ASSERT_EQ(37u, hammingDistance(first, second));
}

TEST(TestHelpers, ShouldAddPaddingWithPKSC7)
{
    std::string input = "YELLOW SU";
    std::string output = "YELLOW SU\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B";
    ASSERT_EQ(output, pkcs7(input, 20));
}
