#include "stream_ciphers.hpp"

#include <string>
#include <iostream>
#include <algorithm>
#include <random>
#include "helpers.hpp"

std::string pkcs7(std::string input, int neededPadding)
{
    input += std::string(neededPadding, neededPadding);
    return input;
}

std::string aes_ecb_decrypt(std::string ciphertext, unsigned char* key, int padding)
{
    std::string decrypted{};
    decrypted.resize(ciphertext.size());
    int out_len1 = decrypted.size();

    EnvCipherCtx ctx{EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free};

    EVP_DecryptInit_ex(ctx.get(), EVP_aes_128_ecb(), NULL, key, NULL);
    EVP_CIPHER_CTX_set_padding(ctx.get(), padding);
    EVP_DecryptUpdate(ctx.get(), as_bytes(decrypted[0]), &out_len1,
                      as_bytes(ciphertext[0]), ciphertext.size());

    int out_len2 = decrypted.size() - out_len1;
    EVP_DecryptFinal_ex(ctx.get(), as_bytes(decrypted[out_len1]), &out_len2);

    decrypted.resize(out_len1 + out_len2);
    return decrypted;
}

std::string aes_ecb_encrypt(std::string plaintext, unsigned char* key, int padding)
{
    std::string encrypted{};
    encrypted.resize(plaintext.size() + 16);
    int out_len1 = encrypted.size();

    EnvCipherCtx ctx{EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free};

    EVP_EncryptInit_ex(ctx.get(), EVP_aes_128_ecb(), NULL, key, NULL);
    EVP_CIPHER_CTX_set_padding(ctx.get(), padding);
    EVP_EncryptUpdate(ctx.get(), as_bytes(encrypted[0]), &out_len1,
                      as_bytes(plaintext[0]), plaintext.size());

    int out_len2 = encrypted.size() - out_len1;
    EVP_EncryptFinal_ex(ctx.get(), as_bytes(encrypted[out_len1]), &out_len2);

    encrypted.resize(out_len1 + out_len2);
    return encrypted;
}

std::string aes_cbc_encrypt(std::string plaintext, unsigned char* key, const std::string& iv)
{
    const int block_size = 16;
    std::string part{iv};
    std::string encrypted{};

    plaintext = pkcs7(plaintext, block_size - (plaintext.size() % block_size));

    for(int i = 0; i < plaintext.size(); i += block_size)
    {
        auto block = plaintext.substr(i, block_size);
        block = fixedXor(block, part);
        part = aes_ecb_encrypt(block, key, 0);
        encrypted += part;
    }
    return encrypted;
}

std::string aes_cbc_decrypt(std::string ciphertext, unsigned char* key, const std::string& iv)
{
    const int block_size = 16;
    std::string prev_iv{iv};
    std::string plaintext;
    for(int i = 0; i < ciphertext.size(); i += block_size)
    {
        auto cipher_block = ciphertext.substr(i, block_size);
        auto decrypted = aes_ecb_decrypt(cipher_block, key, 0);
        plaintext += fixedXor(decrypted, prev_iv);
        prev_iv = cipher_block;
    }
    auto padding = int(plaintext.back());
    plaintext.resize(plaintext.size() - padding);
    return plaintext;
}

std::string random_bytes(int size)
{
    using random_bytes_engine = std::independent_bits_engine<
        std::default_random_engine, CHAR_BIT, unsigned char>;
    std::random_device r{};
    random_bytes_engine rbe{static_cast<unsigned char>(r())};

    std::string ret{};
    std::generate_n(std::back_inserter(ret), size, std::ref(rbe));
    return ret;
}


std::string encryption_oracle(std::string plaintext)
{
    using random_bytes_engine = std::independent_bits_engine<
        std::default_random_engine, CHAR_BIT, unsigned char>;
    std::random_device r{};
    random_bytes_engine rbe{static_cast<unsigned char>(r())};

    unsigned char key[16];
    std::generate_n(key, 16, std::ref(rbe));

    std::mt19937 gen(r());
    std::uniform_int_distribution<> gen_bytes_range(5, 10);

    std::string iv{random_bytes(16)};
    std::string prefix{random_bytes(gen_bytes_range(gen))};
    std::string suffix{random_bytes(gen_bytes_range(gen))};

    std::uniform_int_distribution<> gen_if_ecb(0, 1);
    if(gen_if_ecb(gen))
    {
        std::cout << "ECB" << std::endl;
        return aes_ecb_encrypt(prefix + plaintext + suffix, key);
    }
    std::cout << "CBC" << std::endl;
    return aes_cbc_encrypt(prefix + plaintext + suffix, key, iv);
}
