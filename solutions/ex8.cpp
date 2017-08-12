#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include "helpers.hpp"


using namespace std;

bool hasSameBlocks(string cipher)
{
    double distance = 0;
    int block_size = 16;

    for(int i = 0; i < cipher.size(); i += block_size)
    {
        for(int j = 0; j < cipher.size(); j += block_size)
        {
            if(i != j)
            {
                if(cipher.substr(i, block_size) == cipher.substr(j, block_size))
                    return true;
            }
        }
    }

    return false;
}

int main(int argc, char *argv[])
{
    ifstream infile{"8.txt"};
    string line;
    int lineCount = 1;
    while(getline(infile, line))
    {
        if(hasSameBlocks(hexToByte(line)))
            cout << lineCount << endl;

        lineCount += 1;
    }

}
