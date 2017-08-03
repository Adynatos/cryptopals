#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <map>
#include "helpers.hpp"


using namespace std;

int main(int argc, char *argv[])
{
    ifstream infile{"6.txt"};
    string line;
    string content;
    while(getline(infile, line))
    {
        content += line;
    }

    auto encrypted = base64Decode(content);

    vector<pair<int, double>> keySizeToDistance;

    for(int keysize = 2; keysize <= 40; keysize++)
    {
        auto first = encrypted.substr(0, 4*keysize);
        auto second = encrypted.substr(4*keysize, 4*keysize);

        double dist = hammingDistance(first, second);
        double normalized = dist / keysize;

        keySizeToDistance.emplace_back(keysize, normalized);
    }

    sort(begin(keySizeToDistance), end(keySizeToDistance),
            [](auto  a, auto b)
                { return a.second < b.second; });


    auto keylen = keySizeToDistance[0].first;
    vector<string> transposed{};
    for(int i = 0; i < keylen; ++i)
    {
        transposed.push_back("");
    }
    for(int i = 0; i < encrypted.size(); ++i)
    {
        transposed[i % keylen] += encrypted[i];
    }

    string potentialKey{};
    for(const auto& block : transposed)
    {
        potentialKey.push_back(decodeSingleXorKey(block));
    }

    cout << repeatingXor(encrypted, potentialKey) << endl;
    cout << "Key: " << potentialKey << endl;
}
