#include <iostream>
#include <string>
#include "helpers.hpp"


using namespace std;

int main(int argc, char *argv[])
{
    string input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    cout << base64Encode(hexToByte(input)) << endl;
}
