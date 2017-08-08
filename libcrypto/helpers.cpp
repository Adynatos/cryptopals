#include "helpers.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <limits>
#include <map>
#include <iostream>
#include <bitset>

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

std::string repeatingXor(std::string input, const std::string& key)
{
    auto keyLenght = key.size();
    for(int i = 0; i < input.size(); ++i)
    {
        input[i] ^= key[i % keyLenght];
    }
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

static const std::map<char, double> englishFreq = {
    {'a', 0.08167}, {'b', 0.01492}, {'c', 0.02782}, {'d', 0.04253},
    {'e', 0.12702}, {'f', 0.02228}, {'g', 0.02015}, {'h', 0.06094},
    {'i', 0.06966}, {'j', 0.00153}, {'k', 0.00772}, {'l', 0.04025},
    {'m', 0.02406}, {'n', 0.06749}, {'o', 0.07507}, {'p', 0.01929},
    {'q', 0.00095}, {'r', 0.05987}, {'s', 0.06327}, {'t', 0.09056},
    {'u', 0.02758}, {'v', 0.00978}, {'w', 0.02360}, {'x', 0.00150},
    {'y', 0.01974}, {'z', 0.00074}, {' ', 0.19182}
};


double calculateChiSquared(const std::string& input)
{
    std::map<char, int> count;
    unsigned int ignored = 0;

    for(unsigned int i = 0; i < input.size(); ++i)
    {
        char c = input[i];
        if(c >= 65 && c <= 90)
            count[std::tolower(c)]++;
        else if(c >= 97 && c <= 122)
            count[c]++;
        else if(c == ' ')
            count[c]++;
        else if(c >= 32 && c <= 126)
            ignored++;
        else if(c == 9 || c == 10 || c == 13)
            ignored++;
        else return std::numeric_limits<double>::max(); //Non printable ascii
    }

    double chiSquared = 0.0;
    auto len = input.size() - ignored;
    for(const auto& freq : englishFreq)
    {
        double observed = count[freq.first];
        double expected = len * freq.second;
        double difference = observed - expected;
        chiSquared += difference*difference / expected;
    }
    return chiSquared + ignored * 0.3;
}

std::string decodeSingleXor(const std::string encoded)
{
    auto potentialMessages = std::vector<std::pair<double, std::string>>{};
    for(unsigned char c = 0; c != 255; ++c)
    {
        auto potentialDecoded = singleXor(encoded, c);
        auto score = calculateChiSquared(potentialDecoded);
        potentialMessages.emplace_back(score, potentialDecoded);
    }

    std::sort(std::begin(potentialMessages), std::end(potentialMessages),
         [](auto a, auto b) { return a.first < b.first; });

    return potentialMessages[0].second;
}

char decodeSingleXorKey(const std::string encoded)
{
    return fixedXor(decodeSingleXor(encoded), encoded)[1];
}

//TODO think about loading ints into bitset, find how crypto libs do it
//https://stackoverflow.com/questions/5334268/using-bitwise-operators-in-c-to-change-4-chars-to-int
unsigned int hammingDistance(const std::string& first, const std::string& second)
{
    unsigned int distance = 0;
    for(int i = 0; i < first.size(); ++i)
    {
        auto xored = static_cast<unsigned long>(first[i] ^ second[i]);
        std::bitset<8> b{xored};
        distance += b.count();
    }
    return distance;
}
