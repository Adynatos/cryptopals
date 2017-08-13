#include "stream_ciphers.hpp"

#include <string>

std::string pkcs7(std::string input, int targetSize)
{
    auto neededPadding = targetSize - input.size();
    return input + std::string(neededPadding, neededPadding);
}

constexpr unsigned char* as_bytes(char& str)
{
    return (unsigned char*) &str;
}

std::string aes_ecb_decrypt(std::string ciphertext, unsigned char* key)
{
    std::string decrypted{};
    decrypted.resize(ciphertext.size());

    EnvCipherCtx ctx{EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free};
    int len;
    int plaintext_len;

    EVP_DecryptInit_ex(ctx.get(), EVP_aes_128_ecb(), NULL, key, NULL);
    EVP_DecryptUpdate(ctx.get(), as_bytes(decrypted[0]), &len,
                      as_bytes(ciphertext[0]), ciphertext.size());
    plaintext_len = len;

    EVP_DecryptFinal_ex(ctx.get(), as_bytes(decrypted[len]), &len);
    plaintext_len += len;

    decrypted.resize(plaintext_len);
    return decrypted;
}

std::string aes_ecb_encrypt(std::string plaintext, unsigned char* key)
{
    std::string encrypted{};
    encrypted.resize(plaintext.size() + 16);

    EnvCipherCtx ctx{EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free};
    int len;
    int ciphertext_len;

    EVP_EncryptInit_ex(ctx.get(), EVP_aes_128_ecb(), NULL, key, NULL);
    EVP_EncryptUpdate(ctx.get(), as_bytes(encrypted[0]), &len,
                      as_bytes(plaintext[0]), plaintext.size());
    ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx.get(), as_bytes(encrypted[len]), &len);
    ciphertext_len += len;

    encrypted.resize(ciphertext_len);
    return encrypted;
}
