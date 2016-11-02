#pragma once

#include <string>

std::string hexToByte(const std::string& hexString);
std::string byteToHex(const std::string& byteString);

std::string fixedXor(std::string first, const std::string& second);
std::string singleXor(std::string input, char key);

std::string base64Encode(const std::string& byteString);
std::string base64Decode(const std::string& encoded);

double calculateChiSquared(const std::string& input);
std::string decodeSingleXor(const std::string encoded);
