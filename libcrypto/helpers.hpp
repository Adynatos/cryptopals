#pragma once

#include <string>

std::string hexToByte(const std::string& hexString);
std::string byteToHex(const std::string& byteString);

std::string fixedXor(std::string first, const std::string& second);
std::string singleXor(std::string input, char key);

std::string repeatingXor(std::string input, const std::string& key);

double calculateChiSquared(const std::string& input);
std::string decodeSingleXor(const std::string encoded);
char decodeSingleXorKey(const std::string encoded);

unsigned int hammingDistance(const std::string& first, const std::string& second);
