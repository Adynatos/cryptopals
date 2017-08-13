#pragma once

#include <string>
#include <memory>
#include <openssl/evp.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

std::string pkcs7(std::string input, int targetSize);

using EnvCipherCtx = std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)>;

std::string aes_ecb_decrypt(std::string ciphertext, unsigned char* key);
std::string aes_ecb_encrypt(std::string ciphertext, unsigned char* key);
