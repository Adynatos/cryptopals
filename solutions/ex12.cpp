#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <map>

#include "stream_ciphers.hpp"
#include "base64.hpp"
#include "helpers.hpp"

using namespace std;

string oracle(string plaintext)
{
    static string key = random_bytes(16);

    string secret = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gb"
        "XkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IH"
        "RvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

    return aes_ecb_encrypt(plaintext + base64Decode(secret), as_bytes(key[0]));
}

int main()
{
    auto size = oracle("").size();
    auto block_size = 0;
    for(int i = 1; i < 32; ++i)
    {
        auto tmp_size = oracle(string(i, 'A')).size();
        if(tmp_size != size)
        {
            block_size = tmp_size - size;
            size = tmp_size;
        }
    }
    cout << "block size is: " << block_size << endl;

    string plaintext(64, 'A');
    auto ciphertext = oracle(plaintext);
    if(ciphertext.substr(16, block_size) != ciphertext.substr(32, block_size))
    {
        cout << "ECB mode not detected" << endl;
        return 1;
    }
    cout << "ECB mode detected" << endl;

    auto decrypted = string{};
    auto blocks = oracle(string(block_size-1, 'A')).size() / block_size;
    while(true)
    {
        auto plain = string(block_size - (decrypted.size() % block_size) - 1, 'A');
        map<string, char> potential;
        for(char c = -128; c < 127; ++c)
        {
            auto cipher = oracle(plain + decrypted + c);
            auto first_block = cipher.substr(plain.size(), decrypted.size() + 1);
            potential[first_block] = c;
        }
        auto org = oracle(plain).substr(plain.size(), decrypted.size() + 1);
        auto found = potential.find(org);
        if(potential.find(org) != potential.end())
            decrypted += found->second;
        else
            break;
    }

    cout << "Decrypted: " << decrypted << endl;
}
