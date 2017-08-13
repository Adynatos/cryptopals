#include <gtest/gtest.h>
#include <string>
#include "base64.hpp"

TEST(TestBase64, ShouldEncodeEmptyString)
{
    ASSERT_EQ("", base64Encode(""));
}

TEST(TestBase64, ShouldEncodeByteString)
{
    std::string input = "I'm killing your brain like a poisonous mushroom";
    std::string result = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    ASSERT_EQ(result, base64Encode(input));
}

TEST(TestBase64, ShouldEncodeWithPadding)
{
    ASSERT_EQ("VW5kZXIgVGhlIFN1bg==", base64Encode("Under The Sun"));
    ASSERT_EQ("VGhlIE5vcnRoIFdpbmQ=", base64Encode("The North Wind"));
}

TEST(TestBase64, ShouldDecodeEmptyString)
{
    ASSERT_EQ("", base64Decode(""));
}

TEST(TestBase64, ShouldDecodeBase64String)
{
    std::string input = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    std::string result = "I'm killing your brain like a poisonous mushroom";
    ASSERT_EQ(result, base64Decode(input));
}

TEST(TestBase64, ShouldDecodeWithPadding)
{
    ASSERT_EQ("Under The Sun", base64Decode("VW5kZXIgVGhlIFN1bg=="));
    ASSERT_EQ("The North Wind", base64Decode("VGhlIE5vcnRoIFdpbmQ="));
}
