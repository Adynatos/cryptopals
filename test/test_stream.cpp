#include <gtest/gtest.h>
#include <string>

#include "stream_ciphers.hpp"

TEST(TestStreamCiphers, ShouldAddPaddingWithPKSC7)
{
    std::string input = "YELLOW SU";
    std::string output = "YELLOW SU\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B";
    ASSERT_EQ(output, pkcs7(input, 20));
}

TEST(TestAesEcb, EncryptAndDecryptShouldReturnInput)
{
    std::string input = "But man is not made for defeat. A man can be destroyed but not defeated.";
    unsigned char key[] = "YELLOW SUBMARINE";
    ASSERT_EQ(input, aes_ecb_decrypt(aes_ecb_encrypt(input, key), key));
}
