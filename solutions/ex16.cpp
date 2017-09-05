#include <string>
#include <iostream>

#include "stream_ciphers.hpp"
#include "helpers.hpp"

using namespace std;

string key = random_bytes(16);
string iv = random_bytes(16);

string oracle(string plaintext)
{
    auto prefix = string{"comment1=cooking%20MCs;userdata="};
    auto suffix = string{";comment2=%20like%20a%20pound%20of%20bacon"};

    cout << "prefix size: " << prefix.size() << endl;
    cout << "plaintext size: " << plaintext.size() << endl;
    cout << "suffix size: " << suffix.size() << endl;

    return aes_cbc_encrypt(prefix + plaintext + suffix, as_bytes(key[0]), iv);
}

int main()
{
    auto encrypted = oracle(std::string(16, 'A'));
    auto target = string{"AAAAA;admin=true"};
    cout << "target size: " << target.size() << endl;
    auto fake = encrypted.substr(0, 16) +
        fixedXor(fixedXor(std::string(16, 'A'), target), encrypted.substr(16, 16)) +
        encrypted.substr(32, encrypted.size());

    cout << "decrypted: " << aes_cbc_decrypt(fake, as_bytes(key[0]), iv) << endl;
}
