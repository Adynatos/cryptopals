#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include "helpers.hpp"


using namespace std;

int main(int argc, char *argv[])
{
    ifstream infile{};
    infile.open("4.txt");
    if(!infile.is_open())
    {
        cout << "Unable to open text file" << endl;
        return 1;
    }

    string line;
    vector<pair<double, string>> potential;
    while(getline(infile, line))
    {
        auto decoded = decodeSingleXor(hexToByte(line));
        auto endpos = decoded.find_last_not_of("\n \t");
        if( string::npos != endpos )
        {
                decoded = decoded.substr(0, endpos+1);
        }
        auto score = calculateChiSquared(decoded);
        potential.emplace_back(score, decoded);
    }

    sort(begin(potential), end(potential), [](pair<double, string>  a, pair<double, string> b)
            { return a.first < b.first; });

    cout << "Found potential message: " << potential[0].second << " with score: " << potential[0].first << endl;
}
