#include <gtest/gtest.h>
#include <string>

#include "stream_ciphers.hpp"

TEST(TestStreamCiphers, ShouldAddPaddingWithPKSC7)
{
    std::string input = "YELLOW SU";
    std::string output = "YELLOW SU\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B";
    ASSERT_EQ(output, pkcs7(input, 20));
}
