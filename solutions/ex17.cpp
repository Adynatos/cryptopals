#include <string>
#include <iostream>
#include <algorithm>

#include "stream_ciphers.hpp"
#include "base64.hpp"
#include "helpers.hpp"

using namespace std;

string key = random_bytes(16);
string iv = random_bytes(16);

string oracle()
{

    string input = "MDAwMDAxV2l0aCB0aGUgYmFzcyBraWNrZWQgaW4gYW5kIHRoZSBWZWdhJ3MgYXJlIHB1bXBpbic=";
    input = base64Decode(input);
    cout << "input size: " << input.size() << endl;
    return aes_cbc_encrypt(input, as_bytes(key[0]), iv);
}

bool consume(const string& input)
{
    try
    {
        aes_cbc_decrypt(input, as_bytes(key[0]), iv);
    }
    catch(WrongPaddingException&) {
        return false;
    }
    return true;
}

int main()
{
    auto decrypted = string{""};
    auto cipher = oracle();
    cout << "cipher size: " << cipher.size() << endl;

    string altered = cipher;
    auto start = altered.substr(0, 32);
    auto end = altered.substr(48, 16);

    int i = 1;
    for(unsigned char n = 0; n < 255; ++n)
    {
        string changed = string(16, '\x0');
        changed[16-i] = n;
        auto new_st = start+changed+end;
        if(consume(new_st))
        {
            unsigned char a = i ^ changed[16-i] ^ cipher[48-i];
            decrypted += a;
            break;
        }
    }
    for(i = 2; i < 17; ++i)
    {
        string changed = string(16, '\x0');
        for(int j = 1; j < i; j++)
        {
            changed[16-j] = i ^ decrypted[j-1] ^ cipher[48-j];
        }
        for(unsigned char n = 0; n < 255; ++n)
        {
            changed[16-i] = n;
            auto new_st = start+changed+end;
            if(consume(new_st))
            {
                unsigned char a = i ^ changed[16-i] ^ cipher[48-i];
                decrypted += a;
                break;
            }
        }
    }
    reverse(decrypted.begin(), decrypted.end());
    cout << "decrypted block: " << decrypted << endl;
}
