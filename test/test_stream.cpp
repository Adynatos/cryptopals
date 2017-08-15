#include <gtest/gtest.h>
#include <string>

#include "stream_ciphers.hpp"

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

TEST(TestStreamCiphers, ShouldAddPaddingWithPKSC7)
{
    std::string input = "YELLOW SU";
    std::string output = "YELLOW SU\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B";
    ASSERT_EQ(output, pkcs7(input, 11));
}

TEST(TestStreamCiphers, AesEbcEncryptAndDecryptShouldReturnInput)
{
    std::string input = "But man is not made for defeat. A man can be destroyed but not defeated.";
    unsigned char key[] = "YELLOW SUBMARINE";
    ASSERT_EQ(input, aes_ecb_decrypt(aes_ecb_encrypt(input, key), key));
}

TEST(TestStreamCiphers, AesCbcEncryptAndDecryptShouldReturnInput)
{
    std::string input = "MELLON TAMBURINE";
    unsigned char key[] = "YELLOW SUBMARINE";
    std::string iv = "YELLOW SUBMARINE";
    ASSERT_EQ(input, aes_cbc_decrypt(aes_cbc_encrypt(input, key, iv), key, iv));
}

TEST(TestStreamCiphers, AesCbcEncryptAndDecryptWithPaddingShouldReturnInput)
{
    std::string input = "But man is not made for defeat. A man can be destroyed but not defeated.";
    unsigned char key[] = "YELLOW SUBMARINE";
    std::string iv = "0000000000000000";
    ASSERT_EQ(input, aes_cbc_decrypt(aes_cbc_encrypt(input, key, iv), key, iv));
}
