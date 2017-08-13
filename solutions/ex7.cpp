#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "helpers.hpp"
#include "base64.hpp"
#include "stream_ciphers.hpp"

using namespace::std;




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

    cout << "Decrypted text is:\n" << aes_ecb_decrypt(ciphertext, key);

    EVP_cleanup();
    ERR_free_strings();

    return 0;
}
