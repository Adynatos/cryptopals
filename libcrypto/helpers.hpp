#pragma once

#include <string>
#include <vector>

std::string hexToByte(const std::string& hexString);
std::string byteToHex(const std::string& byteString);

std::string fixedXor(std::string first, const std::string& second);
std::string singleXor(std::string input, char key);

std::string repeatingXor(std::string input, const std::string& key);

double calculateChiSquared(const std::string& input);
std::string decodeSingleXor(const std::string encoded);
char decodeSingleXorKey(const std::string encoded);

unsigned int hammingDistance(const std::string& first, const std::string& second);

std::vector<std::string> split(const std::string& s, char delim);

std::string urlform_encode(const std::string& str);
std::string urlform_decode(const std::string& str);
