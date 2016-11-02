#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string>
#include "helpers.hpp"


using namespace std;

int main(int argc, char *argv[])
{
    auto encoded = std::string{"1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"};
    auto decoded = decodeSingleXor(hexToByte(encoded));

    cout << "Potential decoded message: '" << decoded << "'" << endl;
}
