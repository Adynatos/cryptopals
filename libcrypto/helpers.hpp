#include <string>

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string hexToByteString(const std::string& hexString)
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
