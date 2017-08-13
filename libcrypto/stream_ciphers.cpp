#include "stream_ciphers.hpp"

std::string pkcs7(std::string input, int targetSize)
{
    auto neededPadding = targetSize - input.size();
    return input + std::string(neededPadding, neededPadding);
}
