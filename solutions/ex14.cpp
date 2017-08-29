#include <string>
#include <iostream>
#include <random>
#include <map>
#include "stream_ciphers.hpp"
#include "base64.hpp"

using namespace std;

string oracle(string plaintext)
{
    std::random_device r{};
    std::mt19937 gen(r());
    std::uniform_int_distribution<> gen_bytes_range{0, 1000};

    static string key = random_bytes(16);
    static string secret_prefix = random_bytes(gen_bytes_range(gen));

    string secret = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gb"
        "XkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IH"
        "RvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

    return aes_ecb_encrypt(secret_prefix + plaintext + base64Decode(secret), as_bytes(key[0]));
}

int main()
{
    auto block_size = 16;
    auto prefix = 0;
    auto idx = 0;
    for(int i = 0; i < block_size; ++i)
    {
        auto encrypted = oracle(string(i, 'B') + string(64, 'A'));
        for(int j = 0; j < encrypted.size() - 64; ++j)
        {
            auto first = encrypted.substr(j, block_size);
            auto second = encrypted.substr(j+16, block_size);
            auto third = encrypted.substr(j+32, block_size);
            auto forth = encrypted.substr(j+48, block_size);

            if(first == second && second == third && third == forth)
            {
                cout << "prefix: " << i << " idx: " << j << endl;
                idx = j;
                prefix = i;
            }
        }
    }

    auto start = idx-prefix;

    //TODO extract this into separate function
    auto decrypted = string{};
    while(true)
    {
        auto plain = string(prefix, 'B') + string(block_size - (decrypted.size() % block_size) - 1, 'A');
        map<string, char> potential;
        for(char c = -128; c < 127; ++c)
        {
            auto cipher = oracle(plain + decrypted + c);
            auto first_block = cipher.substr(start + plain.size(), decrypted.size() + 1);
            potential[first_block] = c;
        }
        auto org = oracle(plain).substr(start + plain.size(), decrypted.size() + 1);
        auto found = potential.find(org);
        if(potential.find(org) != potential.end())
            decrypted += found->second;
        else
            break;
    }

    cout << "Decrypted: " << decrypted << endl;


}
