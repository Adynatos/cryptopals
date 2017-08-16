#include "stream_ciphers.hpp"
#include <iostream>

int main()
{
    std::string plaintext('A', 64);
    auto ciphertext = encryption_oracle(plaintext);
    const int block_size = 16;

    if(ciphertext.substr(16, block_size) == ciphertext.substr(32, block_size))
        std::cout << "Encrypted with ECB" << std::endl;
    else
        std::cout << "Encrypted with CBC" << std::endl;

}
