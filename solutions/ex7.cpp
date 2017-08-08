#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "helpers.hpp"

using namespace::std;

using EnvCipherCtx = unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)>;

string decrypt(string ciphertext, unsigned char* key)
{
    string decrypted{};
    decrypted.resize(ciphertext.size());

    EnvCipherCtx ctx{EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free};
    int len;
    int plaintext_len;

    EVP_DecryptInit_ex(ctx.get(), EVP_aes_128_ecb(), NULL, key, NULL);
    EVP_DecryptUpdate(ctx.get(), (unsigned char*) &decrypted[0], &len,
                      (unsigned char*) &ciphertext[0], ciphertext.size());
    plaintext_len = len;

    EVP_DecryptFinal_ex(ctx.get(), (unsigned char*) &decrypted[len], &len);
    plaintext_len += len;

    decrypted.resize(plaintext_len);
    return decrypted;
}

int main()
{
    ifstream infile{"7.txt"};
    string line;
    string content;
    while(getline(infile, line))
    {
        content += line;
    }

    unsigned char key[] = "YELLOW SUBMARINE";
    auto ciphertext = base64Decode(content);

    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);

    cout << "Decrypted text is:\n" << decrypt(ciphertext, key);

    EVP_cleanup();
    ERR_free_strings();

    return 0;
}
