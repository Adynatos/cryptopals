#include "helpers.hpp"
#include "stream_ciphers.hpp"
#include <iostream>
#include <string>

using namespace std;

static auto key = random_bytes(16);

string profile_for(string email)
{
    while(email.find('&') != string::npos)
        email.erase(email.find('&'), 1);
    while(email.find('=') != string::npos)
        email.erase(email.find('='), 1);

    auto url_str = "email=" +email + "&uid=10&role=user";
    return aes_ecb_encrypt(url_str, as_bytes(key[0]));
}

string decode(const string& encrypted)
{
    return aes_ecb_decrypt(encrypted, as_bytes(key[0]));
}


int main()
{
    auto first = profile_for(string{"foooo@bar.com"});
    auto second = profile_for(string{"foo@bar.coadmin"} + string(11, '\x0b')); //fake padding

    cout << decode(first.substr(0, 32) + second.substr(16, 16)) << endl;
}
