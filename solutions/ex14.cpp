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
}
