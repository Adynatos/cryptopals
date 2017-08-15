#pragma once

#include <string>
#include <memory>
#include <openssl/evp.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

std::string pkcs7(std::string input, int targetSize);

using EnvCipherCtx = std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)>;

std::string aes_ecb_decrypt(std::string ciphertext, unsigned char* key, int padding=16);
std::string aes_ecb_encrypt(std::string plaintext, unsigned char* key, int padding=16);

std::string aes_cbc_encrypt(std::string, unsigned char*, const std::string&);
std::string aes_cbc_decrypt(std::string, unsigned char*, const std::string&);
