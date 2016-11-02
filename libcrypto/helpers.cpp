#include "helpers.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string hexToByte(const std::string& hexString)
{
    std::string bytes;
    for(unsigned int i = 0; i < hexString.length(); i += 2)
    {
        std::string byteString = hexString.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

std::string byteToHex(const std::string& byteString)
{
    std::stringstream ss;
    ss << std::hex;
    for(const auto& byte : byteString)
        ss << std::setw(2) << std::setfill('0') << (int)byte;
    return ss.str();
}

std::string fixedXor(std::string first, const std::string& second)
{
    for(unsigned int i = 0; i < first.size(); ++i)
        first[i] ^= second[i];
    return first;
}

std::string singleXor(std::string input, char key)
{
    for(auto& chr : input)
        chr ^= key;
    return input;
}


std::string base64Encode(const std::string& byteString)
{
    std::string result;
    unsigned int neededPadding = 0;
    for(unsigned int i = 0; i < byteString.size(); i += 3)
    {
        auto chunk = byteString.substr(i, 3);
        if(chunk.size() != 3)
        {
            neededPadding = 3 - chunk.size();
            chunk += std::string("\0", neededPadding) ;
        }
        result += base64_chars[(chunk[0] & 0xFC) >> 2];
        result += base64_chars[((chunk[0] & 0x03) << 4) + ((chunk[1] & 0xF0) >> 4)];
        result += base64_chars[((chunk[1] & 0x0F) << 2) + ((chunk[2] & 0xC0) >> 6)];
        result += base64_chars[chunk[2] & 0x3F];
    }

    if(byteString.size() % 3)
    {
        for(unsigned int i = 0; i < neededPadding; ++i)
        {
            result[result.size()-1-i] = '=';
        }
    }

    return result;
}

std::string base64Decode(const std::string& encoded)
{
    std::string result;
    auto paddingSize = std::count(std::begin(encoded), std::end(encoded), '=');

    for(unsigned int i = 0; i < encoded.size(); i += 4)
    {
        auto chunk = encoded.substr(i, 4);

        result += (base64_chars.find(chunk[0]) << 2) +((base64_chars.find(chunk[1]) & 0x30) >> 4);
        result += ((base64_chars.find(chunk[1]) & 0x0F) << 4) + ((base64_chars.find(chunk[2]) & 0x3C) >> 2);
        result += ((base64_chars.find(chunk[2]) & 0x03) << 6) + base64_chars.find(chunk[3]);
    }

    auto sizeWithoutPadding = result.size() - paddingSize;
    result.erase(std::begin(result) + sizeWithoutPadding, std::end(result));

    return result;
}
