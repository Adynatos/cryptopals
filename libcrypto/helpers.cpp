#include "helpers.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <limits>
#include <map>
#include <iostream>
#include <bitset>

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

std::vector<std::string> split(const std::string&s, char delim)
{
    auto result = std::vector<std::string>{};
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while(std::getline(ss, item, delim))
    {
        if(!item.empty())
        {
            result.push_back(item);
        }
    }
    return result;
}

std::string urlform_decode(const std::string& str)
{
    auto start = std::string{"{\n"};
    auto end = std::string{"}"};
    auto middle = std::string{};
    auto parts = split(str, '&');
    for(const auto& part : parts)
    {
        auto kv = split(part, '=');
        middle += ("\t" + kv[0] + ": '" + kv[1] + "',\n"); //TODO use something like .format
    }
    return start + middle + end;
}

std::string urlform_encode(const std::string& str)
{
    auto lines = split(str, '\n');
    auto encoded = std::string{};
    for(int i = 1; i < lines.size() - 1; ++i)
    {
        auto parts = split(lines[i], ':');
        encoded += parts[0].substr(1, parts[0].size());
        encoded += "=";
        encoded += parts[1].substr(2, parts[1].size()-4);
        if(i != lines.size() - 2)
            encoded += "&";
    }
    return encoded;
}
