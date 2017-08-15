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
    ifstream infile{"10.txt"};
    string line;
    string content;
    while(getline(infile, line))
    {
        content += line;
    }

    auto ciphertext = base64Decode(content);
    unsigned char key[] = "YELLOW SUBMARINE";
    std::string iv(16, '\x0'); 

    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);

    cout << "Decrypted text is:\n" << aes_cbc_decrypt(ciphertext, key, iv);

    EVP_cleanup();
    ERR_free_strings();

    return 0;
}
